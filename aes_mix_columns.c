#include "aes_mix_columns.h"

// Galois Field multiplication
static uint8_t gf_mult(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    uint8_t counter;
    uint8_t hi_bit_set;
    
    for (counter = 0; counter < 8; counter++) {
        if (b & 1) {
            p ^= a;
        }
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) {
            a ^= 0x1b; // x^8 + x^4 + x^3 + x + 1
        }
        b >>= 1;
    }
    return p;
}

void mix_columns(state_t state) {
    uint8_t i;
    uint8_t a, b, c, d;
    
    for (i = 0; i < 4; i++) {
        a = state[0][i];
        b = state[1][i];
        c = state[2][i];
        d = state[3][i];
        
        state[0][i] = gf_mult(a, 2) ^ gf_mult(b, 3) ^ c ^ d;
        state[1][i] = a ^ gf_mult(b, 2) ^ gf_mult(c, 3) ^ d;
        state[2][i] = a ^ b ^ gf_mult(c, 2) ^ gf_mult(d, 3);
        state[3][i] = gf_mult(a, 3) ^ b ^ c ^ gf_mult(d, 2);
    }
}

void inv_mix_columns(state_t state) {
    uint8_t i;
    uint8_t a, b, c, d;
    
    for (i = 0; i < 4; i++) {
        a = state[0][i];
        b = state[1][i];
        c = state[2][i];
        d = state[3][i];
        
        state[0][i] = gf_mult(a, 0x0e) ^ gf_mult(b, 0x0b) ^ gf_mult(c, 0x0d) ^ gf_mult(d, 0x09);
        state[1][i] = gf_mult(a, 0x09) ^ gf_mult(b, 0x0e) ^ gf_mult(c, 0x0b) ^ gf_mult(d, 0x0d);
        state[2][i] = gf_mult(a, 0x0d) ^ gf_mult(b, 0x09) ^ gf_mult(c, 0x0e) ^ gf_mult(d, 0x0b);
        state[3][i] = gf_mult(a, 0x0b) ^ gf_mult(b, 0x0d) ^ gf_mult(c, 0x09) ^ gf_mult(d, 0x0e);
    }
}