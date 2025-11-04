#ifndef AES_TYPES_H
#define AES_TYPES_H

#include <stdint.h>

// AES状态矩阵大小
#define AES_BLOCK_SIZE 16
#define AES_KEY_SIZE 16
#define AES_ROUNDS 10

// 状态矩阵类型
typedef uint8_t state_t[4][4];
typedef uint8_t state_t_flat[16];

#endif