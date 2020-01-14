#include "Rudp/Rudp.H"

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
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);
    int len = sizeof(serverAddr);

    if (bind(sock_fd, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind Error!");
        exit(1);
    }

    int recv_num, send_num;
    char send_buf[20480];
    char recv_buf[1024];

    sockaddr_in clientAddr;
    while (true) {
        std::cout << "Server Start at "
                  << Timestamp::LocalTimeFromUniversal(Timestamp::FromRawValue(ptu::get_realtime())).fullString()
                  << endl;
        recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (sockaddr *)&clientAddr, (socklen_t *)&len);
        if (recv_num < 0) {
            perror("recvfrom error ");
            exit(1);
        }
        string message = recv_buf;
        if (message.find("filePath") != string::npos) {
            FileRequest fr;
            deSerialize(message, fr);
            handleFileRequest(sock_fd, clientAddr, fr);
        } else if (message.find("blockID") != string::npos) {
            ResendRequest rr;
            deSerialize(message, rr);
            std::cout << rr.sessionID << std::endl;
        } else {
            perror("Unexcept Struct!\n");
        }
    }
    return 0;
}
