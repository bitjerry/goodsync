#include "crc.h"
#include "base.h"
#include "goodsync.h"
#include <openssl/bn.h>
#include <openssl/evp.h>

int aes_encrypt_master_key(unsigned char *master_key, int master_key_len, unsigned char *out, int *out_len) {
    const static unsigned char salt[] = "Jf5tuaDP";
    const static unsigned char key[] = "CD7qLuyoS9";
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();
    const EVP_MD *md = EVP_sha1();
    const int md_size = EVP_MD_size(md);
    int end_len;
    int count = 10;
    unsigned int crc_value;
    unsigned char _key[64] = {0};
    unsigned char iv[64] = {0};
    unsigned char key_out[1024] = {0x01, 0x00, 0x0a, 0x00, 0x40, 0x00};
    int key_out_len = 0;
    int offset = 6;

    for (int i = 0; i < 8; i++) {
        key_out[offset++] = salt[i];
    }

    if (!EVP_BytesToKey(cipher, md, salt, key, 10, count, _key, iv)) {
        return MASTER_KEY_BYTES_TO_KEY_ERROR;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!EVP_EncryptInit_ex(ctx, cipher, NULL, _key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        return MASTER_KEY_ENCRYPT_INIT_ERROR;
    }

    EVP_MD_CTX *ctx_md = EVP_MD_CTX_create(); // Compatible with openssl 1.0
    if (!EVP_DigestInit(ctx_md, md)) {
        return MASTER_KEY_DIGEST_INIT_ERROR;
    }

    if (!EVP_DigestUpdate(ctx_md, master_key, master_key_len)) {
        return MASTER_KEY_DIGEST_UPDATE_ERROR;
    }

    unsigned char *digest = (unsigned char *) malloc(md_size);

    if (!EVP_DigestFinal_ex(ctx_md, digest, NULL)) {
        free(digest);
        return MASTER_KEY_DIGEST_FINAL_ERROR;
    }
    EVP_MD_CTX_destroy(ctx_md);

    if (!EVP_EncryptUpdate(ctx, key_out + offset, &key_out_len, digest, 8)) {
        free(digest);
        EVP_CIPHER_CTX_free(ctx);
        return MASTER_KEY_ENCRYPT_DIGEST_UPDATE_ERROR;
    }

    free(digest);

    const unsigned char kl[4] = {master_key_len};

    if (!EVP_EncryptUpdate(ctx, key_out + offset + key_out_len, &key_out_len, kl, 4)) {
        EVP_CIPHER_CTX_free(ctx);
        return MASTER_KEY_ENCRYPT_LEN_UPDATE_ERROR;
    }

    if (!EVP_EncryptUpdate(ctx, key_out + offset + key_out_len, &key_out_len, master_key, master_key_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return MASTER_KEY_ENCRYPT_MASTER_KEY_UPDATE_ERROR;
    }

    if (!EVP_EncryptFinal_ex(ctx, key_out + offset + key_out_len, &end_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return MASTER_KEY_ENCRYPT_FINAL_ERROR;
    }

    EVP_CIPHER_CTX_free(ctx);

    key_out_len += offset + end_len;

    crc_value = crc32(key_out, key_out_len);

    for (int i = 0; i < 4; i++) {
        key_out[(key_out_len)++] = (crc_value & (0x000000ff << (8 * i))) >> (8 * i);
    }

    if (base_encode(key_out, key_out_len, out, out_len)) {
        return MASTER_KEY_BASE_ENCODE_ERROR;
    }
    return OK;
}

unsigned char gen_salt_seek(const unsigned char *data, int data_len) {
    unsigned char end = 0;
    for (int i = 0; i < data_len; ++i) {
        end = data[i] + (end >> 1) + (end << 7);
    }
    return end;
}

void gen_salt(unsigned char end, unsigned char *salt) {
    for (int i = 0; i < 8; ++i) {
        salt[i] = (unsigned char) i * '\a' + end;
    }
}

int
aes_encrypt_name(unsigned char *key, int key_len, unsigned char *data, int data_len, unsigned char *out,
                 int *out_len) {
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();
    int end_len, encrypt_data_len;
    unsigned char salt[8] = {0};
    unsigned char _key[64] = {0};
    unsigned char iv[64] = {0};
    unsigned char encrypt_data[1024] = {0};
    unsigned char salt_seek = gen_salt_seek(data, data_len);
    gen_salt(salt_seek, salt);

    if (!EVP_BytesToKey(cipher, EVP_sha1(), salt, key, key_len, 1024, _key, iv)) {
        return FILENAME_ENCRYPT_BYTES_TO_KEY_ERROR;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!EVP_EncryptInit_ex(ctx, cipher, NULL, _key, iv)) {
        return FILENAME_ENCRYPT_INIT_ERROR;
    }

    if (!EVP_EncryptUpdate(ctx, encrypt_data, &encrypt_data_len, data, data_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return FILENAME_ENCRYPT_UPDATE_ERROR;
    }

    if (!EVP_EncryptFinal_ex(ctx, encrypt_data + encrypt_data_len, &end_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return FILENAME_ENCRYPT_FINAL_ERROR;
    }
    EVP_CIPHER_CTX_free(ctx);

    encrypt_data_len += end_len;
    encrypt_data[encrypt_data_len++] = salt_seek;

    if (base_encode(encrypt_data, encrypt_data_len, out, out_len)) {
        return FILENAME_BASE_ENCODE_ERROR;
    }

    return OK;
}

int
aes_decrypt_name(unsigned char *key, int key_len, unsigned char *data, int data_len, unsigned char *out,
                 int *out_len) {
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();
    int end_len, data_out_len;

    unsigned char salt[8] = {0};
    unsigned char _key[64] = {0};
    unsigned char iv[64] = {0};
    unsigned char decode_data[1024] = {0};
    int decode_data_len;

    if (base_decode(data, data_len, decode_data, &decode_data_len)) {
        return FILENAME_BASE_DECODE_ERROR;
    }

    gen_salt(decode_data[decode_data_len - 1], salt);

    if (!EVP_BytesToKey(cipher, EVP_sha1(), salt, key, key_len, 1024, _key, iv)) {
        return FILENAME_DECRYPT_BYTES_TO_KEY_ERROR;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if (!EVP_DecryptInit_ex(ctx, cipher, NULL, _key, iv)) {
        return FILENAME_DECRYPT_INIT_ERROR;
    }

    if (!EVP_DecryptUpdate(ctx, out, &data_out_len, decode_data, decode_data_len - 1)) {
        EVP_CIPHER_CTX_free(ctx);
        return FILENAME_DECRYPT_UPDATE_ERROR;
    }

    if (!EVP_DecryptFinal_ex(ctx, out + data_out_len, &end_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return FILENAME_DECRYPT_FINAL_ERROR;
    }
    EVP_CIPHER_CTX_free(ctx);

    *out_len = data_out_len + end_len;
    return OK;
}

char *message(int code) {
    switch (code) {
        case OK:
            return "Ok";
        case MASTER_KEY_BYTES_TO_KEY_ERROR:
            return "Master key bytes to key error";
        case MASTER_KEY_ENCRYPT_INIT_ERROR:
            return "Master key encrypt init error";
        case MASTER_KEY_DIGEST_INIT_ERROR:
            return "Master key digest init error";
        case MASTER_KEY_DIGEST_UPDATE_ERROR:
            return "Master key digest update error";
        case MASTER_KEY_DIGEST_FINAL_ERROR:
            return "Master key digest final error";
        case MASTER_KEY_ENCRYPT_DIGEST_UPDATE_ERROR:
            return "Master key encrypt digest update error";
        case MASTER_KEY_ENCRYPT_LEN_UPDATE_ERROR:
            return "Master key encrypt len update error";
        case MASTER_KEY_ENCRYPT_MASTER_KEY_UPDATE_ERROR:
            return "Master key encrypt master key update error";
        case MASTER_KEY_ENCRYPT_FINAL_ERROR:
            return "Master key encrypt final error";
        case MASTER_KEY_BASE_ENCODE_ERROR:
            return "Master key base encode error";
        case FILENAME_ENCRYPT_BYTES_TO_KEY_ERROR:
            return "Filename encrypt bytes to key error";
        case FILENAME_ENCRYPT_INIT_ERROR:
            return "Filename encrypt init error";
        case FILENAME_ENCRYPT_UPDATE_ERROR:
            return "Filename encrypt update error";
        case FILENAME_ENCRYPT_FINAL_ERROR:
            return "Filename encrypt final error";
        case FILENAME_BASE_ENCODE_ERROR:
            return "Filename base encode error";
        case FILENAME_DECRYPT_BYTES_TO_KEY_ERROR:
            return "Filename decrypt bytes to key error";
        case FILENAME_DECRYPT_INIT_ERROR:
            return "Filename decrypt init error";
        case FILENAME_DECRYPT_UPDATE_ERROR:
            return "Filename decrypt update error";
        case FILENAME_DECRYPT_FINAL_ERROR:
            return "Filename decrypt final error";
        case FILENAME_BASE_DECODE_ERROR:
            return "Filename base decode error";
        default:
            return "Oops!!!";
    }

}
