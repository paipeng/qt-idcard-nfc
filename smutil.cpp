#include "smutil.h"
#include <QDebug>

SMUtil::SMUtil()
{

}

void SMUtil::setSM4Key(unsigned char *key) {
    qDebug() << "setSM4Key";
    memcpy(this->sm4Key, key, sizeof(unsigned char) * SM4_KEY_SIZE);
}
char* SMUtil::sm4Encrypt(unsigned char* data, int data_len) {
    qDebug() << "sm4Encrypt: " << data_len;
    sm4_setkey_enc(&this->ctx, this->sm4Key);
    // sm4_crypt_ecb(&ctx, 1, 16, input, output);
    return NULL;
}
char* SMUtil::sm4Decrypt(unsigned char* data, int data_len) {
    qDebug() << "sm4Decrypt: " << data_len;
    sm4_setkey_dec(&ctx, this->sm4Key);
    // sm4_crypt_ecb(&ctx, 0, 16, output, output);
    return NULL;
}
