packet FileTransferMsg {
    // Message types
    int msgType;           // 1=READ_REQUEST (download)
                          // 2=DATA
                          // 3=ACK
                          // 4=ERROR
                          // 5=KEY_EXCHANGE
                          // 6=WRITE_REQUEST (upload)
    
    // Transfer info
    int seqNum = 0;
    int totalChunks = 0;
    string filename;
    int fileSize = 0;
    int chunkSize = 0;
    string data;
    bool isLastChunk = false;
    
    // Encryption fields
    bool encrypted = false;
    int encryptionKey = 0;
    string checksum;
    int originalSize = 0;
    
    // Client identification
    int clientId = 0;      // Identifies which client
}