#include "smutil.h"
#include <QDebug>

SMUtil::SMUtil()
{

}

void SMUtil::setSM4Key(unsigned char *key) {
    qDebug() << "setSM4Key";
    memcpy(this->sm4Key, key, sizeof(unsigned char) * SM4_BLOCK_SIZE);
}

unsigned char* SMUtil::sm4Encrypt(unsigned char* data, int data_len, int *encoded_data_len) {
    qDebug() << "sm4Encrypt: " << data_len;
    sm4_setkey_enc(&this->ctx, this->sm4Key);


    // sm4_crypt_ecb(&ctx, SM4_ENCRYPT, 16, input, output);



    *encoded_data_len = (data_len % SM4_BLOCK_SIZE == 0)?data_len:(data_len/SM4_BLOCK_SIZE+1)*SM4_BLOCK_SIZE;
    qDebug() << "encoded_data_len: " << *encoded_data_len;

    unsigned char* encoded_data = (unsigned char*) malloc( sizeof (unsigned char) * *encoded_data_len);

    for (int i = 0; i < (*encoded_data_len); i+= SM4_BLOCK_SIZE) {
        if ((i + SM4_BLOCK_SIZE) > (*encoded_data_len)) {
            unsigned char block_data[SM4_BLOCK_SIZE];
            memset(block_data, 0, sizeof(unsigned char) * SM4_BLOCK_SIZE);
            memcpy(block_data, data+i, sizeof(unsigned char) * (*encoded_data_len - i));
            sm4_crypt_ecb(&ctx, SM4_ENCRYPT, SM4_BLOCK_SIZE, block_data, (encoded_data+i));
        } else {
            sm4_crypt_ecb(&ctx, SM4_ENCRYPT, SM4_BLOCK_SIZE, (data + i), (encoded_data+i));
        }
    }
    return encoded_data;
}

unsigned char* SMUtil::sm4Decrypt(unsigned char* data, int data_len, int decoded_data_len) {
    qDebug() << "sm4Decrypt: " << data_len;
    sm4_setkey_dec(&ctx, this->sm4Key);
    // sm4_crypt_ecb(&ctx, SM4_DECRYPT, 16, output, output);

    unsigned char* decoded_data = (unsigned char*) malloc( sizeof (unsigned char) * decoded_data_len);

    for (int i = 0; i < data_len; i+= SM4_BLOCK_SIZE) {
        if ((i + SM4_BLOCK_SIZE) >= data_len) {
            unsigned char block_data[SM4_BLOCK_SIZE];
            memset(block_data, 0, sizeof(unsigned char) * SM4_BLOCK_SIZE);
            sm4_crypt_ecb(&ctx, SM4_DECRYPT, SM4_BLOCK_SIZE, block_data, block_data);

            memcpy(decoded_data+i, block_data, sizeof(unsigned char) * ( SM4_BLOCK_SIZE - (data_len - decoded_data_len)));
        } else {
            sm4_crypt_ecb(&ctx, SM4_DECRYPT, SM4_BLOCK_SIZE, (data + i), (decoded_data+i));
        }
    }

    return decoded_data;
}
