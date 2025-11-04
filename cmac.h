#ifndef CMAC_H
#define CMAC_H

#include "aes_types.h"

void aes_cmac(const uint8_t *key, const uint8_t *message, 
              uint32_t message_len, uint8_t *mac);

// 添加调试函数声明
void generate_subkeys_debug(const uint8_t *key, uint8_t *k1, uint8_t *k2);

#endif