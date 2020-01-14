#ifndef _Rudp_H_
#define _Rudp_H_

#include <arpa/inet.h>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <map>
#include <netinet/in.h>
#include <openssl/sha.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Postman/PostUtility.H"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace std;
using namespace rapidjson;

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 55555
#define SHARED_PATH "/data/MarketData/RealtimeRecord/CNZZF/2019/"

struct ErrorMessage {
    string packetHash = "";
    uint16_t packetLength = 128;
    uint8_t msgType = 0xFF;
    uint8_t appID = 0x01;
    uint8_t appMsgType = 0xFF;
    uint8_t errorCode = 0;
    string sessionID = "";
    int8_t appErrorCode = 0;
    string message = "";
};

struct FileRequest {
    string packetHash = "";
    uint16_t packetLength = 128;
    uint8_t msgType = 0x03;
    uint8_t appID = 0x01;
    uint8_t appMsgType = 0x01;
    uint8_t errorCode = 0;
    string sessionID = "";
    uint16_t blockSize = 8192;
    string timeStamp = "";
    string filePath = "";
};

struct FileSummary {
    string packetHash = "";
    uint16_t packetLength = 128;
    uint8_t msgType = 0x03;
    uint8_t appID = 0x01;
    uint8_t appMsgType = 0x12;
    uint8_t errorCode = 0;
    string sessionID = "";
    string timeStamp = "0";
    uint16_t blockCount = 0;
    uint32_t fileSize = 0;
    uint16_t blockSize = 8192;
    string fileName = "";
};

struct DataBlock {
    string packetHash = "";
    uint16_t packetLength = 16512;
    uint8_t msgType = 0x01;
    uint8_t appID = 0x01;
    uint8_t appMsgType = 0x13;
    uint8_t errorCode = 0;
    string sessionID = "";
    uint16_t blockID = 0;
    string data = "";
};

struct ResendRequest {
    string packetHash = "";
    uint16_t packetLength = 128;
    uint8_t msgType = 0x03;
    uint8_t appID = 0x01;
    uint8_t appMsgType = 0x14;
    uint8_t errorCode = 0;
    string sessionID = "";
    uint16_t blockID = 0; // 0 -> FileSummary; 1 - blockSize -> DataBlock
};

string serialize(ErrorMessage &em);
string serialize(FileRequest &fr);
string serialize(FileSummary &fs);
string serialize(DataBlock &db);
string serialize(ResendRequest &rr);

void deSerialize(string message, ErrorMessage &em);
void deSerialize(string message, FileRequest &fr);
void deSerialize(string message, FileSummary &fs);
void deSerialize(string message, DataBlock &db);
void deSerialize(string message, ResendRequest &rr);

template <typename T>
void sendMessage(int sock_fd, sockaddr_in addr, T &t) {
    string message = serialize(t);
    sendto(sock_fd, message.data(), message.length(), 0, (sockaddr *)&addr, sizeof(addr));
}

void getDataBlocks(FileRequest &fr, map<uint16_t, string> dataBlocks);
string hashPacket(DataBlock &db);
void handleFileRequest(int, sockaddr_in, FileRequest &);

#endif
