#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

using namespace std;

class EncryptionUtil {
public:
    static string encrypt(const string& data, int key);
    static string decrypt(const string& data, int key);
    static int generateSessionKey();
    static string calculateChecksum(const string& data);
    static bool verifyChecksum(const string& data, const string& checksum);

private:
    static string xorEncrypt(const string& data, int key);
};

#endif
