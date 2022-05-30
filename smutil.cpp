#include "smutil.h"
#include <QDebug>

static unsigned char SM4_KEY[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

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
    memset(decoded_data, 0, sizeof (unsigned char) * decoded_data_len);
    for (int i = 0; i < data_len; i+= SM4_BLOCK_SIZE) {
        if ((i + SM4_BLOCK_SIZE) > data_len) {
            qDebug() << "last block";
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

void SMUtil::test() {
    setSM4Key(SM4_KEY);
    int data_len = 3450;
    unsigned char* input = (unsigned char*) malloc(sizeof(unsigned char) * data_len);
    for (int i = 0; i < data_len; i++) {
        input[i] = i % 256;
    }

    qDebug() << "input data: " << convertToHexString(input, data_len);

    int encoded_data_len = 0;
    unsigned char *encoded_data = sm4Encrypt(input, data_len, &encoded_data_len);

    qDebug() << "encoded_data_len: " << encoded_data_len;

    if (encoded_data != NULL) {
        qDebug() << "encoded_data data: " << convertToHexString(encoded_data, encoded_data_len);
        unsigned char *decoded_data = sm4Decrypt(encoded_data, encoded_data_len, data_len);
        if (decoded_data != NULL) {

            qDebug() << "decoded data: " << convertToHexString(decoded_data, data_len);
            for (int i = 0; i < data_len; i++) {
                if (input[i] != decoded_data[i]) {
                    qDebug() << "sm4 check error! "<< i;
                }
            }

            free(decoded_data);
        }


        free(encoded_data);
    }

}

QString SMUtil::convertToHexString(unsigned char* data, int data_len) {
    QString text;
    for (int i = 0; i < 16; i++) {
        QString c;
        c.sprintf("0x%02X ", data[i]);
        text.append(c);
    }
    return text;
}
