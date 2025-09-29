/*
 * multi_server.cc
 *
 *  Created on: Sep 29, 2025
 *      Author: mmahm
 */

#include <omnetpp.h>
#include "../FileTransferMsg_m.h"
#include "encryption.h"
#include <map>

using namespace omnetpp;
using namespace std;

enum TransferMode {
    DOWNLOAD,  // Server sends file to client
    UPLOAD     // Client sends file to server
};

struct ClientSession {
    string filename;
    string fileData;
    int totalChunks;
    int currentChunk;
    int chunkSize;
    int sessionKey;
    bool encryptionEnabled;
    TransferMode mode;
    int clientId;
    bool active;
};

class MultiServer : public cSimpleModule {
private:
    int chunkSize;
    bool enableEncryption;
    map<int, ClientSession> clientSessions;  // clientId -> session
    map<string, string> fileStorage;         // filename -> file content

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    // Download (read) operations
    void handleReadRequest(FileTransferMsg *msg, int clientId);
    void sendKeyExchange(int clientId);
    void sendNextDownloadChunk(int clientId);

    // Upload (write) operations
    void handleWriteRequest(FileTransferMsg *msg, int clientId);
    void handleUploadData(FileTransferMsg *msg, int clientId);
    void saveUploadedFile(int clientId);

    // Common operations
    void handleAck(FileTransferMsg *msg, int clientId);
    string generateDummyFile(const string& filename, int size);
    int getClientIdFromGate(cGate* gate);
};

Define_Module(MultiServer);

void MultiServer::initialize() {
    chunkSize = par("chunkSize");
    enableEncryption = par("enableEncryption");

    // Pre-populate some files in storage
    fileStorage["readme.txt"] = generateDummyFile("readme.txt", 1024);
    fileStorage["data.csv"] = generateDummyFile("data.csv", 2048);
    fileStorage["image.jpg"] = generateDummyFile("image.jpg", 3072);

    EV << "🌐 Multi-Client Server initialized" << endl;
    EV << "📁 " << fileStorage.size() << " files available for download" << endl;
    EV << "🔒 Encryption: " << (enableEncryption ? "ON" : "OFF") << endl;
}

int MultiServer::getClientIdFromGate(cGate* gate) {
    return gate->getIndex();
}

void MultiServer::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        // Timer for sending next chunk
        FileTransferMsg *ftMsg = check_and_cast<FileTransferMsg *>(msg);
        int clientId = ftMsg->getClientId();

        if (clientSessions[clientId].active &&
            clientSessions[clientId].mode == DOWNLOAD &&
            clientSessions[clientId].currentChunk < clientSessions[clientId].totalChunks) {
            sendNextDownloadChunk(clientId);
        }
        delete msg;
        return;
    }

    FileTransferMsg *ftMsg = check_and_cast<FileTransferMsg *>(msg);
    int clientId = getClientIdFromGate(msg->getArrivalGate());

    if (ftMsg->getMsgType() == 1) { // READ_REQUEST (download)
        handleReadRequest(ftMsg, clientId);
    } else if (ftMsg->getMsgType() == 6) { // WRITE_REQUEST (upload)
        handleWriteRequest(ftMsg, clientId);
    } else if (ftMsg->getMsgType() == 2) { // DATA (from client upload)
        handleUploadData(ftMsg, clientId);
    } else if (ftMsg->getMsgType() == 3) { // ACK
        handleAck(ftMsg, clientId);
    }

    delete ftMsg;
}

void MultiServer::handleReadRequest(FileTransferMsg *msg, int clientId) {
    string filename = msg->getFilename();

    EV << "📥 Client " << clientId << " requests download: " << filename << endl;

    // Check if file exists
    if (fileStorage.find(filename) == fileStorage.end()) {
        EV << "❌ File not found: " << filename << endl;
        FileTransferMsg *error = new FileTransferMsg("Error");
        error->setMsgType(4);
        error->setData("File not found");
        send(error, "out", clientId);
        return;
    }

    // Setup download session
    ClientSession session;
    session.clientId = clientId;
    session.filename = filename;
    session.fileData = fileStorage[filename];
    session.chunkSize = chunkSize;
    session.totalChunks = (session.fileData.length() + chunkSize - 1) / chunkSize;
    session.currentChunk = 0;
    session.mode = DOWNLOAD;
    session.active = true;

    if (enableEncryption) {
        session.sessionKey = EncryptionUtil::generateSessionKey();
        session.encryptionEnabled = true;
        EV << "🔑 Generated key for client " << clientId << ": " << session.sessionKey << endl;
    } else {
        session.encryptionEnabled = false;
    }

    clientSessions[clientId] = session;

    if (session.encryptionEnabled) {
        sendKeyExchange(clientId);
    } else {
        sendNextDownloadChunk(clientId);
    }
}

void MultiServer::handleWriteRequest(FileTransferMsg *msg, int clientId) {
    string filename = msg->getFilename();
    int fileSize = msg->getFileSize();

    EV << "📤 Client " << clientId << " wants to upload: " << filename
       << " (" << fileSize << " bytes)" << endl;

    // Setup upload session
    ClientSession session;
    session.clientId = clientId;
    session.filename = filename;
    session.fileData = "";  // Will accumulate uploaded data
    session.chunkSize = chunkSize;
    session.totalChunks = (fileSize + chunkSize - 1) / chunkSize;
    session.currentChunk = 0;
    session.mode = UPLOAD;
    session.active = true;

    if (enableEncryption) {
        session.sessionKey = EncryptionUtil::generateSessionKey();
        session.encryptionEnabled = true;
        EV << "🔑 Generated key for client " << clientId << ": " << session.sessionKey << endl;
    } else {
        session.encryptionEnabled = false;
    }

    clientSessions[clientId] = session;

    if (session.encryptionEnabled) {
        sendKeyExchange(clientId);
    } else {
        // Send ACK to start upload
        FileTransferMsg *ack = new FileTransferMsg("UploadReady");
        ack->setMsgType(3);
        ack->setSeqNum(-2); // Special ACK for upload ready
        send(ack, "out", clientId);
    }
}

void MultiServer::handleUploadData(FileTransferMsg *msg, int clientId) {
    if (clientSessions.find(clientId) == clientSessions.end()) return;

    ClientSession &session = clientSessions[clientId];
    string receivedData = msg->getData();
    string finalData = receivedData;

    // Decrypt if needed
    if (msg->getEncrypted() && session.encryptionEnabled) {
        finalData = EncryptionUtil::decrypt(receivedData, session.sessionKey);
        EV << "🔓 Client " << clientId << " - Decrypted chunk " << msg->getSeqNum() << endl;
    }

    // Accumulate data
    session.fileData += finalData;
    session.currentChunk++;

    EV << "📥 Client " << clientId << " - Received chunk "
       << msg->getSeqNum() << "/" << session.totalChunks << endl;

    // Send ACK
    FileTransferMsg *ack = new FileTransferMsg("UploadACK");
    ack->setMsgType(3);
    ack->setSeqNum(msg->getSeqNum());
    send(ack, "out", clientId);

    // Check if upload complete
    if (msg->isLastChunk()) {
        saveUploadedFile(clientId);
    }
}

void MultiServer::saveUploadedFile(int clientId) {
    ClientSession &session = clientSessions[clientId];
    fileStorage[session.filename] = session.fileData;

    EV << "💾 Client " << clientId << " - Upload complete: " << session.filename << endl;
    EV << "📊 Saved " << session.fileData.length() << " bytes" << endl;
    EV << "📁 Total files in storage: " << fileStorage.size() << endl;

    session.active = false;
}

void MultiServer::sendKeyExchange(int clientId) {
    ClientSession &session = clientSessions[clientId];

    FileTransferMsg *keyMsg = new FileTransferMsg("KeyExchange");
    keyMsg->setMsgType(5);
    keyMsg->setEncryptionKey(session.sessionKey);
    keyMsg->setFilename(session.filename.c_str());
    keyMsg->setTotalChunks(session.totalChunks);
    keyMsg->setClientId(clientId);

    EV << "🔑 Sending key to client " << clientId << endl;
    send(keyMsg, "out", clientId);
}

void MultiServer::handleAck(FileTransferMsg *msg, int clientId) {
    if (clientSessions.find(clientId) == clientSessions.end()) return;

    ClientSession &session = clientSessions[clientId];

    if (msg->getSeqNum() == -1) {
        // Key exchange ACK
        EV << "🔑 Client " << clientId << " - Key confirmed" << endl;

        if (session.mode == DOWNLOAD) {
            sendNextDownloadChunk(clientId);
        } else {
            // Send upload ready ACK
            FileTransferMsg *ack = new FileTransferMsg("UploadReady");
            ack->setMsgType(3);
            ack->setSeqNum(-2);
            send(ack, "out", clientId);
        }
        return;
    }

    if (session.mode == DOWNLOAD) {
        EV << "✅ Client " << clientId << " - ACK for chunk " << msg->getSeqNum() << endl;

        if (session.currentChunk < session.totalChunks) {
            FileTransferMsg *timer = new FileTransferMsg("downloadTimer");
            timer->setClientId(clientId);
            scheduleAt(simTime() + par("processingDelay"), timer);
        } else {
            EV << "🎉 Client " << clientId << " - Download complete!" << endl;
            session.active = false;
        }
    }
}

void MultiServer::sendNextDownloadChunk(int clientId) {
    ClientSession &session = clientSessions[clientId];

    int startPos = session.currentChunk * session.chunkSize;
    int remainingBytes = session.fileData.length() - startPos;
    int currentChunkSize = min(session.chunkSize, remainingBytes);

    string chunkData = session.fileData.substr(startPos, currentChunkSize);
    string finalData = chunkData;
    string checksum;

    if (session.encryptionEnabled) {
        finalData = EncryptionUtil::encrypt(chunkData, session.sessionKey);
        checksum = EncryptionUtil::calculateChecksum(chunkData);
        EV << "🔒 Client " << clientId << " - Encrypting chunk " << session.currentChunk << endl;
    }

    FileTransferMsg *dataMsg = new FileTransferMsg("DownloadData");
    dataMsg->setMsgType(2);
    dataMsg->setSeqNum(session.currentChunk);
    dataMsg->setTotalChunks(session.totalChunks);
    dataMsg->setFilename(session.filename.c_str());
    dataMsg->setChunkSize(finalData.length());
    dataMsg->setOriginalSize(chunkData.length());
    dataMsg->setData(finalData.c_str());
    dataMsg->setEncrypted(session.encryptionEnabled);
    dataMsg->setChecksum(checksum.c_str());
    dataMsg->setIsLastChunk(session.currentChunk == session.totalChunks - 1);
    dataMsg->setClientId(clientId);

    EV << "📤 Client " << clientId << " - Sending chunk "
       << session.currentChunk << "/" << session.totalChunks << endl;

    send(dataMsg, "out", clientId);
    session.currentChunk++;
}

string MultiServer::generateDummyFile(const string& filename, int size) {
    string content = "📄 File: " + filename + " | This is sample content. ";
    string result;

    while (result.length() < size) {
        result += content;
    }

    return result.substr(0, size);
}


