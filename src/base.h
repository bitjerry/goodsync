//
// Created by Mr.lin on 2024-03-25.
//

#ifndef GOODSYNC_BASE_H
#define GOODSYNC_BASE_H

#define BIT 6
#define DIFFER 2

int base_encode(unsigned char *p_str, int len, unsigned char *out, int *out_len);

int base_decode(unsigned char *p_str, int len, unsigned char *out, int *out_len);

#endif //GOODSYNC_BASE_H
