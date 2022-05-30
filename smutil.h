#ifndef SMUTIL_H
#define SMUTIL_H

#include <QObject>
#include "sm4.h"
#define SM4_KEY_SIZE 16

class SMUtil
{
public:
    SMUtil();
    void setSM4Key(unsigned char *key);
    char* sm4Encrypt(unsigned char* data, int data_len);
    char* sm4Decrypt(unsigned char* data, int data_len);
private:
    unsigned char sm4Key[SM4_KEY_SIZE];
    sm4_context ctx;
};

#endif // SMUTIL_H
