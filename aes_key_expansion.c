#include "aes_key_expansion.h"
#include "aes_sbox.h"

// Rcon table
static const uint8_t Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

void key_expansion(const uint8_t *key, uint8_t *round_keys)
{
    int i, j;
    uint8_t temp[4];

    // First round key is the original key 第一轮密钥就是原始密钥
    for (i = 0; i < 16; i++)
    {
        round_keys[i] = key[i];
    }

    // Generate subsequent round keys 生成后续轮密钥
    for (i = 16; i < 176; i += 4)
    {
        // Read previous 4 bytes 读取前4个字节
        for (j = 0; j < 4; j++)
        {
            temp[j] = round_keys[i - 4 + j];
        }

        // Core transformation every 16 bytes 进行核心变换，每16字节一次
        if (i % 16 == 0)
        {
            // Rotate left
            uint8_t t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;

            // S-box substitution  S-box替换
            for (j = 0; j < 4; j++)
            {
                temp[j] = sbox_lookup(temp[j]);
            }

            // Rcon
            temp[0] ^= Rcon[(i / 16) - 1];
        }

        // XOR operation 异或操作
        for (j = 0; j < 4; j++)
        {
            round_keys[i + j] = round_keys[i - 16 + j] ^ temp[j];
        }
    }
}

uint8_t *get_round_key(uint8_t *round_keys, int round)
{
    return round_keys + (round * 16);
}