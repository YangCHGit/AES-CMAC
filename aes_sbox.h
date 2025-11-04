#ifndef AES_SBOX_H
#define AES_SBOX_H

#include "aes_types.h"

// S盒查找
uint8_t sbox_lookup(uint8_t byte);
uint8_t inv_sbox_lookup(uint8_t byte);

// S盒变换
void sub_bytes(state_t state);
void inv_sub_bytes(state_t state);

#endif