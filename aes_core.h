#ifndef AES_CORE_H
#define AES_CORE_H

#include "aes_types.h"

void aes_encrypt(const uint8_t *input, uint8_t *output, const uint8_t *round_keys);
void aes_decrypt(const uint8_t *input, uint8_t *output, const uint8_t *round_keys);

#endif