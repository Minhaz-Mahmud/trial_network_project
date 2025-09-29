/*
 * multi_client.cc
 *
 *  Created on: Sep 29, 2025
 *      Author: mmahm
 */

// Fixed src/Modules/multi_client.cc
#include <omnetpp.h>
#include "../FileTransferMsg_m.h"
#include "encryption.h"

using namespace omnetpp;
using namespace std;

enum ClientMode {
    DOWNLOADING,
    UPLOADING,
    IDLE
};

class MultiClient : public cSimpleModule {
private:
    string filename;
    int fileSize;
    ClientMode mode;

    // Download state
    int expectedChunks;
    int receivedChunks;
    string reconstructedFile;

    // Upload state
    string uploadData;
    int uploadChunks;
    int sentChunks;

    // Encryption
    int sessionKey;
    bool encryptionActive;
    bool keyReceived;
    int chunkSize;
    bool uploadStarted;  // NEW: Track if upload has started

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    // Download operations
    void sendReadRequest();
    void handleDownloadData(FileTransferMsg *msg);

    // Upload operations
    void sendWriteRequest();
    void sendNextUploadChunk();
    string generateUploadData(int size);

    // Common operations
    void handleKeyExchange(FileTransferMsg *msg);
};

Define_Module(MultiClient);

void MultiClient::initialize() {
    filename = par("filename").stdstringValue();
    fileSize = par("fileSize");
    string modeStr = par("mode").stdstringValue();
    chunkSize = par("chunkSize");

    // Determine mode
    if (modeStr == "upload") {
        mode = UPLOADING;
    } else {
        mode = DOWNLOADING;
    }

    expectedChunks = 0;
    receivedChunks = 0;
    sentChunks = 0;
    sessionKey = 0;
    encryptionActive = false;
    keyReceived = false;
    uploadStarted = false;  // Initialize upload flag

    double delay = par("requestDelay");
    scheduleAt(simTime() + delay, new cMessage("startTransfer"));
}

void MultiClient::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        // Handle self messages
        if (strcmp(msg->getName(), "startTransfer") == 0) {
            // Initial transfer start
            if (mode == DOWNLOADING) {
                sendReadRequest();
            } else if (mode == UPLOADING) {
                sendWriteRequest();
            }
        } else if (strcmp(msg->getName(), "sendNext") == 0) {
            // Send next upload chunk
            sendNextUploadChunk();
        }
        delete msg;
    } else {
        FileTransferMsg *ftMsg = check_and_cast<FileTransferMsg *>(msg);

        if (ftMsg->getMsgType() == 5) { // KEY_EXCHANGE
            handleKeyExchange(ftMsg);
        } else if (ftMsg->getMsgType() == 2) { // DATA (download)
            handleDownloadData(ftMsg);
        } else if (ftMsg->getMsgType() == 3) { // ACK
            if (ftMsg->getSeqNum() == -2) {
                // Upload ready signal
                EV << "✅ Server ready for upload" << endl;
                uploadStarted = true;  // Mark upload as started
                sendNextUploadChunk();
            } else if (mode == UPLOADING && uploadStarted) {
                // ACK for uploaded chunk
                EV << "✅ Chunk " << ftMsg->getSeqNum() << " acknowledged" << endl;
                if (sentChunks < uploadChunks) {
                    // Schedule next chunk
                    scheduleAt(simTime() + 0.1, new cMessage("sendNext"));
                } else {
                    EV << "🎉 Upload complete!" << endl;
                    mode = IDLE;
                    uploadStarted = false;
                }
            }
        } else if (ftMsg->getMsgType() == 4) { // ERROR
            EV << "❌ Error: " << ftMsg->getData() << endl;
        }

        delete ftMsg;
    }
}

void MultiClient::sendReadRequest() {
    FileTransferMsg *request = new FileTransferMsg("ReadRequest");
    request->setMsgType(1); // READ_REQUEST
    request->setFilename(filename.c_str());

    EV << "📥 Requesting download: " << filename << endl;
    send(request, "out");
}

void MultiClient::sendWriteRequest() {
    // Only send write request if not already uploading
    if (uploadStarted) {
        return;
    }

    uploadData = generateUploadData(fileSize);
    uploadChunks = (uploadData.length() + chunkSize - 1) / chunkSize;
    sentChunks = 0;

    FileTransferMsg *request = new FileTransferMsg("WriteRequest");
    request->setMsgType(6); // WRITE_REQUEST
    request->setFilename(filename.c_str());
    request->setFileSize(fileSize);

    EV << "📤 Requesting upload: " << filename << " (" << fileSize << " bytes)" << endl;
    send(request, "out");
}

void MultiClient::handleKeyExchange(FileTransferMsg *msg) {
    sessionKey = msg->getEncryptionKey();
    encryptionActive = true;
    keyReceived = true;

    if (mode == DOWNLOADING) {
        expectedChunks = msg->getTotalChunks();
        EV << "🔑 Received encryption key: " << sessionKey << " for download" << endl;
    } else {
        EV << "🔑 Received encryption key: " << sessionKey << " for upload" << endl;
    }

    FileTransferMsg *ack = new FileTransferMsg("KeyACK");
    ack->setMsgType(3);
    ack->setSeqNum(-1);
    send(ack, "out");
}

void MultiClient::handleDownloadData(FileTransferMsg *msg) {
    string receivedData = msg->getData();
    string finalData = receivedData;

    if (msg->getEncrypted() && encryptionActive) {
        finalData = EncryptionUtil::decrypt(receivedData, sessionKey);
        EV << "🔓 Decrypted chunk " << msg->getSeqNum() << endl;
    }

    reconstructedFile += finalData;
    receivedChunks++;

    EV << "📥 Received chunk " << msg->getSeqNum() << "/" << expectedChunks << endl;

    FileTransferMsg *ack = new FileTransferMsg("ChunkACK");
    ack->setMsgType(3);
    ack->setSeqNum(msg->getSeqNum());
    send(ack, "out");

    if (msg->isLastChunk()) {
        EV << "🎉 Download complete! Received " << reconstructedFile.length() << " bytes" << endl;
        mode = IDLE;
        reconstructedFile.clear();
        receivedChunks = 0;
        expectedChunks = 0;
        encryptionActive = false;
    }
}

void MultiClient::sendNextUploadChunk() {
    // Safety check
    if (!uploadStarted || sentChunks >= uploadChunks) {
        return;
    }

    int startPos = sentChunks * chunkSize;
    int remainingBytes = uploadData.length() - startPos;
    int currentChunkSize = min(chunkSize, remainingBytes);

    string chunkData = uploadData.substr(startPos, currentChunkSize);
    string finalData = chunkData;
    string checksum;

    if (encryptionActive) {
        finalData = EncryptionUtil::encrypt(chunkData, sessionKey);
        checksum = EncryptionUtil::calculateChecksum(chunkData);
        EV << "🔒 Encrypting chunk " << sentChunks << endl;
    }

    FileTransferMsg *dataMsg = new FileTransferMsg("UploadData");
    dataMsg->setMsgType(2);
    dataMsg->setSeqNum(sentChunks);
    dataMsg->setTotalChunks(uploadChunks);
    dataMsg->setFilename(filename.c_str());
    dataMsg->setChunkSize(finalData.length());
    dataMsg->setData(finalData.c_str());
    dataMsg->setEncrypted(encryptionActive);
    dataMsg->setChecksum(checksum.c_str());
    dataMsg->setIsLastChunk(sentChunks == uploadChunks - 1);

    EV << "📤 Uploading chunk " << sentChunks << "/" << uploadChunks << endl;

    send(dataMsg, "out");
    sentChunks++;
}

string MultiClient::generateUploadData(int size) {
    string content = "🔼 UPLOAD DATA from " + getFullPath() + " | ";
    content += "This is client-generated content for upload. ";
    string result;

    while (result.length() < size) {
        result += content;
    }

    return result.substr(0, size);
}


