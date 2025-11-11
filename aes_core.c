#include "aes_core.h"
#include "aes_utils.h"
#include "aes_sbox.h"
#include "aes_shift_rows.h"
#include "aes_mix_columns.h"
#include "aes_key_expansion.h"

static void add_round_key(state_t state, const uint8_t *round_key)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[i][j] ^= round_key[i + j * 4];
        }
    }
}

void aes_encrypt(const uint8_t *input, uint8_t *output, const uint8_t *round_keys) {
    state_t state;
    
    // Initial state
    flat_to_state(input, state);
    
    // Initial round key addition
    add_round_key(state, get_round_key((uint8_t *)round_keys, 0));
    
    // 9 main rounds
    for (int round = 1; round < AES_ROUNDS; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, get_round_key((uint8_t *)round_keys, round));
    }
    
    // Final round (no mix columns)
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, get_round_key((uint8_t *)round_keys, AES_ROUNDS));
    
    // Output result
    state_to_flat(state, output);
}

void aes_decrypt(const uint8_t *input, uint8_t *output, const uint8_t *round_keys) {
    state_t state;
    
    // Initial state
    flat_to_state(input, state);
    
    // Initial round key addition
    add_round_key(state, get_round_key((uint8_t *)round_keys, AES_ROUNDS));
    
    // 9 main rounds
    for (int round = AES_ROUNDS - 1; round > 0; round--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, get_round_key((uint8_t *)round_keys, round));
        inv_mix_columns(state);
    }
    
    // Final round (no mix columns)
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, get_round_key((uint8_t *)round_keys, 0));
    
    // Output result
    state_to_flat(state, output);
}