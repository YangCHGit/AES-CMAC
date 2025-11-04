#include "aes_utils.h"
#include <stdio.h>

void print_state(const state_t state) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02x ", state[j][i]);
        }
        printf("\n");
    }
}

void print_state_flat(const uint8_t *state) {
    for (int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
        if ((i + 1) % 4 == 0) printf(" ");
    }
    printf("\n");
}

void state_to_flat(const state_t state, uint8_t *output) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            output[i * 4 + j] = state[j][i];
        }
    }
}

void flat_to_state(const uint8_t *input, state_t state) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[j][i] = input[i * 4 + j];
        }
    }
}