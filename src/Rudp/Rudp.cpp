#include "Rudp.H"
#include <iostream>

using namespace std;

std::map<string, std::map<uint16_t, string>> contents;

string serialize(ErrorMessage &em) {
    StringBuffer buf;
    Writer<StringBuffer> writer(buf);
    writer.StartObject();

    writer.Key("packetHash");
    writer.String("");
    writer.Key("packetLength");
    writer.Uint(em.packetLength);
    writer.Key("msgType");
    writer.Uint(em.msgType);
    writer.Key("appID");
    writer.Uint(em.appID);
    writer.Key("appMsgType");
    writer.Uint(em.appMsgType);
    writer.Key("errorCode");
    writer.Uint(em.errorCode);
    writer.Key("sessionID");
    writer.String((const char *const &)em.sessionID);
    writer.Key("appErrorCode");
    writer.Uint(em.appErrorCode);
    writer.Key("message");
    writer.String((const char *const &)em.message);

    writer.EndObject();

    return buf.GetString();
}

void deSerialize(string message, ErrorMessage &em) {
    Document doc;
    if (!doc.Parse(message.data()).HasParseError()) {
        em.packetHash = doc["packetHash"].GetString();
        em.packetLength = doc["packetLength"].GetUint();
        em.msgType = doc["msgType"].GetUint();
        em.appID = doc["appID"].GetUint();
        em.appMsgType = doc["appMsgType"].GetUint();
        em.errorCode = doc["errorCode"].GetUint();
        em.sessionID = doc["sessionID"].GetString();
        em.appErrorCode = doc["appErrorCode"].GetUint();
        em.message = doc["message"].GetString();
    } else {
        perror("deSerialize ErrorMessage Fail!");
    }
}

string serialize(FileRequest &fr) {
    StringBuffer buf;
    Writer<StringBuffer> writer(buf);
    writer.StartObject();

    writer.Key("packetHash");
    writer.String("");
    writer.Key("packetLength");
    writer.Uint(fr.packetLength);
    writer.Key("msgType");
    writer.Uint(fr.msgType);
    writer.Key("appID");
    writer.Uint(fr.appID);
    writer.Key("appMsgType");
    writer.Uint(fr.appMsgType);
    writer.Key("errorCode");
    writer.Uint(fr.errorCode);
    writer.Key("sessionID");
    writer.String((const char *const &)fr.sessionID);
    writer.Key("blockSize");
    writer.Uint(fr.blockSize);
    writer.Key("timeStamp");
    writer.String((const char *const &)fr.timeStamp);
    writer.Key("filePath");
    writer.String((const char *const &)fr.filePath);

    writer.EndObject();

    return buf.GetString();
}

void deSerialize(string message, FileRequest &fr) {
    Document doc;
    if (!doc.Parse(message.data()).HasParseError()) {
        fr.packetHash = doc["packetHash"].GetString();
        fr.packetLength = doc["packetLength"].GetUint();
        fr.msgType = doc["msgType"].GetUint();
        fr.appID = doc["appID"].GetUint();
        fr.appMsgType = doc["appMsgType"].GetUint();
        fr.errorCode = doc["errorCode"].GetUint();
        fr.sessionID = doc["sessionID"].GetString();
        fr.blockSize = doc["blockSize"].GetUint();
        fr.timeStamp = doc["timeStamp"].GetString();
        fr.filePath = doc["filePath"].GetString();
    } else {
        perror("deSerialize FileRequest Fail!");
    }
}

string serialize(FileSummary &fs) {
    StringBuffer buf;
    Writer<StringBuffer> writer(buf);
    writer.StartObject();

    writer.Key("packetHash");
    writer.String("");
    writer.Key("packetLength");
    writer.Uint(fs.packetLength);
    writer.Key("msgType");
    writer.Uint(fs.msgType);
    writer.Key("appID");
    writer.Uint(fs.appID);
    writer.Key("appMsgType");
    writer.Uint(fs.appMsgType);
    writer.Key("errorCode");
    writer.Uint(fs.errorCode);
    writer.Key("sessionID");
    writer.String((const char *const &)fs.sessionID);
    writer.Key("timeStamp");
    writer.String((const char *const &)fs.timeStamp);
    writer.Key("blockCount");
    writer.Uint(fs.blockCount);
    writer.Key("fileSize");
    writer.Uint(fs.fileSize);
    writer.Key("blockSize");
    writer.Uint(fs.blockSize);
    writer.Key("fileName");
    writer.String((const char *const &)fs.fileName);

    writer.EndObject();

    return buf.GetString();
}

void deSerialize(string message, FileSummary &fs) {
    Document doc;
    if (!doc.Parse(message.data()).HasParseError()) {
        fs.packetHash = doc["packetHash"].GetString();
        fs.packetLength = doc["packetLength"].GetUint();
        fs.msgType = doc["msgType"].GetUint();
        fs.appID = doc["appID"].GetUint();
        fs.appMsgType = doc["appMsgType"].GetUint();
        fs.errorCode = doc["errorCode"].GetUint();
        fs.sessionID = doc["sessionID"].GetString();
        fs.timeStamp = doc["timeStamp"].GetString();
        fs.blockCount = doc["blockCount"].GetUint();
        fs.fileSize = doc["fileSize"].GetUint();
        fs.blockSize = doc["blockSize"].GetUint();
        fs.fileName = doc["fileName"].GetString();
    } else {
        perror("deSerialize FileSummary Fail!");
    }
}

string serialize(DataBlock &db) {
    StringBuffer buf;
    Writer<StringBuffer> writer(buf);
    writer.StartObject();

    writer.Key("packetHash");
    writer.String((const char *const &)db.packetHash);
    writer.Key("packetLength");
    writer.Uint(db.packetLength);
    writer.Key("msgType");
    writer.Uint(db.msgType);
    writer.Key("appID");
    writer.Uint(db.appID);
    writer.Key("appMsgType");
    writer.Uint(db.appMsgType);
    writer.Key("errorCode");
    writer.Uint(db.errorCode);
    writer.Key("sessionID");
    writer.String((const char *const &)db.sessionID);
    writer.Key("blockID");
    writer.Uint(db.blockID);
    writer.Key("data");
    writer.String((const char *const &)db.data);

    writer.EndObject();

    return buf.GetString();
}

void deSerialize(string message, DataBlock &db) {
    Document doc;
    if (!doc.Parse(message.data()).HasParseError()) {
        db.packetHash = doc["packetHash"].GetString();
        db.packetLength = doc["packetLength"].GetUint();
        db.msgType = doc["msgType"].GetUint();
        db.appID = doc["appID"].GetUint();
        db.appMsgType = doc["appMsgType"].GetUint();
        db.sessionID = doc["sessionID"].GetString();
        db.blockID = doc["blockID"].GetUint();
        db.data = doc["data"].GetString();
    } else {
        perror("deSerialize DataBlock Fail!");
    }
}

string serialize(ResendRequest &rr) {
    StringBuffer buf;
    Writer<StringBuffer> writer(buf);
    writer.StartObject();

    writer.Key("packetHash");
    writer.String("");
    writer.Key("packetLength");
    writer.Uint(rr.packetLength);
    writer.Key("msgType");
    writer.Uint(rr.msgType);
    writer.Key("appID");
    writer.Uint(rr.appID);
    writer.Key("appMsgType");
    writer.Uint(rr.appMsgType);
    writer.Key("errorCode");
    writer.Uint(rr.errorCode);
    writer.Key("sessionID");
    writer.String((const char *const &)rr.sessionID);
    writer.Key("blockID");
    writer.Uint(rr.blockID);

    writer.EndObject();

    return buf.GetString();
}

void deSerialize(string message, ResendRequest &rr) {
    Document doc;
    if (!doc.Parse(message.data()).HasParseError()) {
        rr.packetHash = doc["packetHash"].GetString();
        rr.packetLength = doc["packetLength"].GetUint();
        rr.msgType = doc["msgType"].GetUint();
        rr.appID = doc["appID"].GetUint();
        rr.appMsgType = doc["appMsgType"].GetUint();
        rr.sessionID = doc["sessionID"].GetString();
        rr.blockID = doc["blockID"].GetUint();
    } else {
        perror("deSerialize ResendRequest Fail!");
    }
}

uint32_t getContents(FileRequest &fr, map<uint16_t, string> &contents) {
    ifstream fin(SHARED_PATH + fr.filePath);
    ostringstream tmp;
    tmp << fin.rdbuf();
    string data = tmp.str();
    uint32_t fileSize = data.length();
    uint16_t count = ceil(fileSize / (float)fr.blockSize);
    for (uint16_t index = 0; index < count - 1; ++index) {
        contents[index + 1] = data.substr(fr.blockSize * index, fr.blockSize);
    }
    auto left = fileSize - fr.blockSize * (count - 1) + 1;
    contents[count] = data.substr(fr.blockSize * (count - 1), left);
    return count;
}

string hashPacket(DataBlock &db) {
    string data = db.data;
    const char *d = db.data.c_str();
    SHA_CTX ctx;
    unsigned char md[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)d, strlen(d), md);
    string res;
    for (size_t i = 0; i < SHA_DIGEST_LENGTH; i++) {
        char tmp[10];
        sprintf(tmp, "%02x", md[i]);
        res += tmp;
    }
    return res;
}

void handleFileRequest(int sock_fd, sockaddr_in addr, FileRequest &fr) {
    struct stat buf;
    string filepath = SHARED_PATH + fr.filePath;
    stat(filepath.c_str(), &buf);
    Uint64 ts = (((Uint64)buf.st_mtim.tv_sec << 32) | ((Uint64)buf.st_mtim.tv_nsec));
    string fileTimeStamp = Datestamp::FromNanoTime(Timestamp::FromRawValue(ts)).toString() + " " +
                           Timestamp::LocalTimeFromUniversal(Timestamp::FromRawValue(ts)).fullString();
    if (fr.timeStamp == "0" || fr.timeStamp <= fileTimeStamp) {
        uint16_t index = 0;
        FileSummary fs;
        fs.sessionID = fr.sessionID;
        fs.timeStamp = fileTimeStamp;
        fs.fileSize = buf.st_size;
        fs.fileName = fr.filePath;
        map<uint16_t, string> content;
        fs.blockCount = getContents(fr, content);
        contents[fr.sessionID] = content;
        sendMessage<FileSummary>(sock_fd, addr, fs);
        index++;
        while (index <= fs.blockCount) {
            DataBlock db;
            db.sessionID = fr.sessionID;
            db.blockID = index;
            db.data = content[index];
            db.packetHash = hashPacket(db);
            sendMessage(sock_fd, addr, db);
            usleep(2000);
            index++;
        }
    } else {
        return;
    }
}
