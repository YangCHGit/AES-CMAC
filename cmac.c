#include "cmac.h"
#include "aes_core.h"
#include "aes_key_expansion.h"
#include <string.h>

// Left shift one bit in a 16-byte array
static void left_shift_one_bit(const uint8_t *input, uint8_t *output) {
    uint8_t overflow = 0;
    uint8_t new_overflow;
    
    for (int i = 15; i >= 0; i--) {
        new_overflow = (input[i] & 0x80) ? 1 : 0;
        output[i] = (input[i] << 1) | overflow;
        overflow = new_overflow;
    }
}

// XOR two 16-byte arrays
static void xor_blocks(const uint8_t *a, const uint8_t *b, uint8_t *result) {
    for (int i = 0; i < 16; i++) {
        result[i] = a[i] ^ b[i];
    }
}

// Generate subkeys K1 and K2 for CMAC
static void generate_subkeys(const uint8_t *key, uint8_t *k1, uint8_t *k2) {
    uint8_t l[16] = {0};
    uint8_t const_Rb[16] = {0};
    const_Rb[15] = 0x87;  // Rb = 0x87
    
    uint8_t round_keys[176];
    
    // Expand key
    key_expansion(key, round_keys);
    
    // Encrypt zero block to get L
    aes_encrypt(l, l, round_keys);
    
    // Generate K1
    left_shift_one_bit(l, k1);
    if (l[0] & 0x80) {
        xor_blocks(k1, const_Rb, k1);
    }
    
    // Generate K2
    left_shift_one_bit(k1, k2);
    if (k1[0] & 0x80) {
        xor_blocks(k2, const_Rb, k2);
    }
}

// Debug version for testing
void generate_subkeys_debug(const uint8_t *key, uint8_t *k1, uint8_t *k2) {
    generate_subkeys(key, k1, k2);
}

void aes_cmac(const uint8_t *key, const uint8_t *message, 
              uint32_t message_len, uint8_t *mac) {
    uint8_t k1[16], k2[16];
    uint8_t x[16] = {0};  // Initial vector (zero)
    uint8_t y[16];
    uint8_t m_last[16];
    uint8_t padded[16] = {0};
    
    uint32_t n = (message_len + 15) / 16;  // Number of blocks
    int need_padding = (message_len == 0) || (message_len % 16 != 0);
    
    uint8_t round_keys[176];
    
    // Generate subkeys
    generate_subkeys(key, k1, k2);
    
    // Expand key
    key_expansion(key, round_keys);
    
    if (n == 0) {
        n = 1;
    }
    
    // Process first n-1 blocks
    for (uint32_t i = 0; i < n - 1; i++) {
        xor_blocks(&message[i * 16], x, y);
        aes_encrypt(y, x, round_keys);
    }
    
    // Process last block
    if (need_padding) {
        // Last block is incomplete, need padding
        uint32_t remaining = message_len % 16;
        if (remaining == 0 && message_len > 0) {
            remaining = 16;
        }
        
        if (message_len > 0) {
            memcpy(padded, &message[(n - 1) * 16], remaining);
        }
        padded[remaining] = 0x80;  // Add padding bit
        
        xor_blocks(padded, k2, m_last);
    } else {
        // Last block is complete
        xor_blocks(&message[(n - 1) * 16], k1, m_last);
    }
    
    // Final encryption
    xor_blocks(m_last, x, y);
    aes_encrypt(y, mac, round_keys);
}