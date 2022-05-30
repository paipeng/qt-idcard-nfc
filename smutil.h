#ifndef SMUTIL_H
#define SMUTIL_H

#include <QObject>
#include "sm4.h"
#define SM4_BLOCK_SIZE 16

class SMUtil
{
public:
    SMUtil();
    void setSM4Key(unsigned char *key);
    unsigned char* sm4Encrypt(unsigned char* data, int data_len, int *encoded_data_len);
    unsigned char* sm4Decrypt(unsigned char* data, int data_len, int decoded_data_len);
    void test();
    QString convertToHexString(unsigned char* data, int data_len);
private:
    unsigned char sm4Key[SM4_BLOCK_SIZE];
    sm4_context ctx;
};

#endif // SMUTIL_H
