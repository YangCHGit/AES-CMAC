#ifndef AES_KEY_EXPANSION_H
#define AES_KEY_EXPANSION_H

#include "aes_types.h"

void key_expansion(const uint8_t *key, uint8_t *round_keys);
uint8_t *get_round_key(uint8_t *round_keys, int round);

#endif