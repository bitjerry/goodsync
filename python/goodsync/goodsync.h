//
// Created by Mr.lin on 2024-03-31.
//

#ifndef GOODSYNC_GOODSYNC_H
#define GOODSYNC_GOODSYNC_H

#define OK 0
#define MASTER_KEY_BYTES_TO_KEY_ERROR 1
#define MASTER_KEY_ENCRYPT_INIT_ERROR 2
#define MASTER_KEY_DIGEST_INIT_ERROR 3
#define MASTER_KEY_DIGEST_UPDATE_ERROR 4
#define MASTER_KEY_DIGEST_FINAL_ERROR 5
#define MASTER_KEY_ENCRYPT_DIGEST_UPDATE_ERROR 6
#define MASTER_KEY_ENCRYPT_LEN_UPDATE_ERROR 7
#define MASTER_KEY_ENCRYPT_MASTER_KEY_UPDATE_ERROR 8
#define MASTER_KEY_ENCRYPT_FINAL_ERROR 9
#define MASTER_KEY_BASE_ENCODE_ERROR 10
#define FILENAME_ENCRYPT_BYTES_TO_KEY_ERROR 11
#define FILENAME_ENCRYPT_INIT_ERROR 12
#define FILENAME_ENCRYPT_UPDATE_ERROR 13
#define FILENAME_ENCRYPT_FINAL_ERROR 14
#define FILENAME_BASE_ENCODE_ERROR 15
#define FILENAME_DECRYPT_BYTES_TO_KEY_ERROR 16
#define FILENAME_DECRYPT_INIT_ERROR 17
#define FILENAME_DECRYPT_UPDATE_ERROR 18
#define FILENAME_DECRYPT_FINAL_ERROR 19
#define FILENAME_BASE_DECODE_ERROR 20

char *message(int code);

int aes_encrypt_master_key(unsigned char *master_key, int master_key_len, unsigned char *out, int *out_len);

int
aes_encrypt_name(unsigned char *key, int key_len, unsigned char *data, int data_len, unsigned char *out, int *out_len);

int
aes_decrypt_name(unsigned char *key, int key_len, unsigned char *data, int data_len, unsigned char *out, int *out_len);

#endif //GOODSYNC_GOODSYNC_H
