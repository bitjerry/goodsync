//
// Created by Mr.lin on 2024-03-31.
//

#include <stdio.h>
#include "goodsync.h"

int main() {
    unsigned char master_key[] = "0123456789";
    unsigned char data[] = "test";  //gs_zipfs_state
    unsigned char key[1024] = {0};

    int key_len;
    int result = aes_encrypt_master_key(master_key, sizeof(master_key) - 1, key, &key_len);
    if (result) {
        printf("%s\n", message(result));
    }

    printf("Master key len: %d\n", key_len);
    printf("Master key: ");
    for (int i = 0; i < key_len; ++i) {
        printf("%c", key[i]);
    }
    printf("\n\n");

    unsigned char encrypt_data[1024] = {0};
    int encrypt_data_len;
    int data_len = sizeof(data) - 1;
    result = aes_encrypt_name(key, key_len, data, data_len, encrypt_data, &encrypt_data_len);
    if (result) {
        printf("%s\n", message(result));
    }

    printf("Source data len: %d\n", data_len);
    printf("Source data: ");
    for (int i = 0; i < data_len; ++i) {
        printf("%02x ", data[i]);
    }
    printf("\n\n");
    printf("Encrypt data len: %d\n", encrypt_data_len);
    printf("Encrypt data: ");
    for (int i = 0; i < encrypt_data_len; ++i) {
        printf("%c", encrypt_data[i]);
    }
    printf("\n\n");

    unsigned char decrypt_data[1024] = {0};
    int decrypt_data_len;
    result = aes_decrypt_name(key, key_len, encrypt_data, encrypt_data_len, decrypt_data, &decrypt_data_len);
    if (result) {
        printf("%s\n", message(result));
    }

    printf("Decrypt data len: %llu\n", sizeof(decrypt_data) - 1);
    printf("Decrypt data: ");
    for (int i = 0; i < decrypt_data_len; ++i) {
        printf("%02x ", decrypt_data[i]);
    }
    printf("\n");

    return 0;
}
