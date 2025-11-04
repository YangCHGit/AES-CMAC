#ifndef AES_UTILS_H
#define AES_UTILS_H

#include "aes_types.h"

// 打印状态矩阵
void print_state(const state_t state);
void print_state_flat(const uint8_t *state);

// 数据转换
void state_to_flat(const state_t state, uint8_t *output);
void flat_to_state(const uint8_t *input, state_t state);

#endif