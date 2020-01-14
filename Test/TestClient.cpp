#include "Rudp/Rudp.H"
#include <fstream>
#include <thread>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("Create sock_fd Error!");
        exit(1);
    }
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serverAddr.sin_port = htons(SERVER_PORT);
    int len = sizeof(serverAddr);

    int recv_num, send_num;
    char recv_buf[20480];
    char send_buf[1024];

    string fileTimeStamp = Datestamp::FromNanoTime(Timestamp::FromRawValue(ptu::get_realtime())).toString() + " " +
                           Timestamp::LocalTimeFromUniversal(Timestamp::FromRawValue(ptu::get_realtime())).fullString();
    FileRequest fr;
    fr.sessionID = fileTimeStamp;
    fr.timeStamp = "20191204 12:10:01.123456789";
    fr.filePath = "realtimedata-20191210-1.csv";
    string frjson = serialize(fr);
    std::cout << frjson << std::endl;
    sendMessage(sock_fd, serverAddr, fr);

    std::map<uint16_t, string> content;
    std::vector<std::string> Messages;
    int count = 0;
    int blockCount = 0;
    while (true) {
        recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (sockaddr *)&serverAddr, (socklen_t *)&len);
        string message = recv_buf;
        if (message.find("appErrorCode") != string::npos) {
            ErrorMessage em;
            deSerialize(message, em);
            std::cout << em.message << std::endl;
        } else if (message.find("blockCount") != string::npos) {
            FileSummary fs;
            deSerialize(message, fs);
            std::cout << message << std::endl;
            blockCount = fs.blockCount;
        } else if (message.find("blockID") != string::npos) {
            DataBlock db;
            int end = message.find_first_of("}");
            message = message.substr(0, end + 1);
            deSerialize(message, db);
            string clientHash = hashPacket(db);
            if (db.packetHash == clientHash) {
                count++;
                std::cout << count << " " << db.blockID << std::endl;
                content[db.blockID] = db.data;
            } else {
                std::cout << message << std::endl;
            }
        } else {
            std::cout << message << std::endl;
            perror("Unexcept Struct!\n");
        }

        if (count == blockCount) {
            break;
        }
    }
    ofstream f("/home/xiaowei/Projects/bbudp/" + fr.filePath, ios::out);
    for (int i = 1; i <= blockCount; ++i) {
        f << content[i];
    }
    f.close();
    return 0;
}
