#include "encryption.h"
#include <sstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

string EncryptionUtil::encrypt(const string& data, int key) {
    return xorEncrypt(data, key);
}

string EncryptionUtil::decrypt(const string& data, int key) {
    return xorEncrypt(data, key);
}

string EncryptionUtil::xorEncrypt(const string& data, int key) {
    string result = data;
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = result[i] ^ (key + i % 256);
    }
    return result;
}

int EncryptionUtil::generateSessionKey() {
    return 1000 + (rand() % 9000);
}

string EncryptionUtil::calculateChecksum(const string& data) {
    int sum = 0;
    for (char c : data) {
        sum += static_cast<unsigned char>(c);
    }

    stringstream ss;
    ss << hex << (sum % 65516);
    return ss.str();
}

bool EncryptionUtil::verifyChecksum(const string& data, const string& checksum) {
    return calculateChecksum(data) == checksum;
}
