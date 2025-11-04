#include <stdio.h>
#include <string.h>
#include "cmac.h"
#include "aes_utils.h"
#include "aes_core.h"
#include "aes_key_expansion.h"

void print_hex(const char *label, const uint8_t *data, int len) {
    printf("%s: ", label);
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
        if (i < len - 1) printf(" ");
    }
    printf("\n");
}

// XOR function for debugging
static void debug_xor_blocks(const uint8_t *a, const uint8_t *b, uint8_t *result) {
    for (int i = 0; i < 16; i++) {
        result[i] = a[i] ^ b[i];
    }
}

// Left shift for debugging
static void debug_left_shift_one_bit(const uint8_t *input, uint8_t *output) {
    uint8_t overflow = 0;
    uint8_t new_overflow;
    
    for (int i = 15; i >= 0; i--) {
        new_overflow = (input[i] & 0x80) ? 1 : 0;
        output[i] = (input[i] << 1) | overflow;
        overflow = new_overflow;
    }
}

void test_case_1() {
    printf("=== Test Case 1 (Your Provided Test Vector) ===\n");
    
    uint8_t key[AES_KEY_SIZE] = {
        0x47, 0x65, 0x65, 0x71, 0x76, 0x6E, 0x4B, 0x45, 
        0x68, 0x6A, 0x37, 0x38, 0x6B, 0x63, 0x31, 0x50
    };
    
    uint8_t message[] = {
        0x57, 0xE1, 0x33, 0x12, 0x77, 0xC7, 0x9E, 0x82, 
        0x3B, 0xA4, 0x48, 0x6F, 0xA6, 0xFF, 0xAC, 0xF5
    };
    
    uint32_t message_len = 16;
    uint8_t mac[AES_BLOCK_SIZE];
    uint8_t expected_mac[AES_BLOCK_SIZE] = {
        0xF9, 0x4C, 0x60, 0x0F, 0x66, 0x02, 0x28, 0x9D, 
        0x35, 0xCC, 0x8D, 0x19, 0x21, 0x0C, 0x9A, 0xA2
    };
    
    print_hex("Key", key, AES_KEY_SIZE);
    print_hex("Message", message, message_len);
    
    aes_cmac(key, message, message_len, mac);
    
    print_hex("Computed CMAC", mac, AES_BLOCK_SIZE);
    print_hex("Expected CMAC", expected_mac, AES_BLOCK_SIZE);
    
    if (memcmp(mac, expected_mac, AES_BLOCK_SIZE) == 0) {
        printf("RESULT: PASSED!\n");
    } else {
        printf("RESULT: FAILED!\n");
        
        // Debug: Let's check intermediate values
        printf("\nDebug Information:\n");
        
        uint8_t k1[16], k2[16];
        uint8_t round_keys[176];
        uint8_t zero_block[16] = {0};
        uint8_t l[16];
        
        key_expansion(key, round_keys);
        aes_encrypt(zero_block, l, round_keys);
        print_hex("L (E_K(0))", l, 16);
        
        // Generate subkeys manually for debugging
        uint8_t const_Rb[16] = {0};
        const_Rb[15] = 0x87;
        
        debug_left_shift_one_bit(l, k1);
        if (l[0] & 0x80) {
            debug_xor_blocks(k1, const_Rb, k1);
        }
        print_hex("K1", k1, 16);
        
        debug_left_shift_one_bit(k1, k2);
        if (k1[0] & 0x80) {
            debug_xor_blocks(k2, const_Rb, k2);
        }
        print_hex("K2", k2, 16);
        
        // Test the subkey generation function
        uint8_t k1_test[16], k2_test[16];
        generate_subkeys_debug(key, k1_test, k2_test);
        print_hex("K1 (from function)", k1_test, 16);
        print_hex("K2 (from function)", k2_test, 16);
        
        // Test encryption of the message
        uint8_t encrypted[16];
        aes_encrypt(message, encrypted, round_keys);
        print_hex("E_K(M)", encrypted, 16);
    }
    printf("\n");
}

void test_case_2() {
    printf("=== Test Case 2 (NIST Test Vector) ===\n");
    
    uint8_t key[AES_KEY_SIZE] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    
    uint8_t message[] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
    };
    
    uint32_t message_len = 16;
    uint8_t mac[AES_BLOCK_SIZE];
    uint8_t expected_mac[AES_BLOCK_SIZE] = {
        0x07, 0x0a, 0x16, 0xb4, 0x6b, 0x4d, 0x41, 0x44,
        0xf7, 0x9b, 0xdd, 0x9d, 0xd0, 0x4a, 0x28, 0x7c
    };
    
    print_hex("Key", key, AES_KEY_SIZE);
    print_hex("Message", message, message_len);
    
    aes_cmac(key, message, message_len, mac);
    
    print_hex("Computed CMAC", mac, AES_BLOCK_SIZE);
    print_hex("Expected CMAC", expected_mac, AES_BLOCK_SIZE);
    
    if (memcmp(mac, expected_mac, AES_BLOCK_SIZE) == 0) {
        printf("RESULT: PASSED!\n");
    } else {
        printf("RESULT: FAILED!\n");
    }
    printf("\n");
}

void test_case_3() {
    printf("=== Test Case 3 (Empty Message) ===\n");
    
    uint8_t key[AES_KEY_SIZE] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    
    uint8_t message[] = {0};  // Empty message
    uint32_t message_len = 0;
    uint8_t mac[AES_BLOCK_SIZE];
    uint8_t expected_mac[AES_BLOCK_SIZE] = {
        0xbb, 0x1d, 0x69, 0x29, 0xe9, 0x59, 0x37, 0x28,
        0x7f, 0xa3, 0x7d, 0x12, 0x9b, 0x75, 0x67, 0x46
    };
    
    print_hex("Key", key, AES_KEY_SIZE);
    printf("Message: (empty)\n");
    
    aes_cmac(key, message, message_len, mac);
    
    print_hex("Computed CMAC", mac, AES_BLOCK_SIZE);
    print_hex("Expected CMAC", expected_mac, AES_BLOCK_SIZE);
    
    if (memcmp(mac, expected_mac, AES_BLOCK_SIZE) == 0) {
        printf("RESULT: PASSED!\n");
    } else {
        printf("RESULT: FAILED!\n");
    }
    printf("\n");
}



void test_case_4() {
    printf("=== Test Case 4 (Your Provided Test Vector) ===\n");
    
    uint8_t key[AES_KEY_SIZE] = {
        0x65, 0x33, 0x48, 0x68, 0x2F, 0x47, 0x2B, 0x45, 
        0x69, 0x48, 0x49, 0x65, 0x56, 0x68, 0x39, 0x52
    };
    
    uint8_t message[] = {
        0x97, 0x31, 0x73, 0x52, 0xB7, 0xF7, 0xCE, 0xC2,
        0x7B, 0x34, 0x68, 0x63, 0x06, 0x2F, 0x4C, 0x65
    };
    
    uint32_t message_len = 16;
    uint8_t mac[AES_BLOCK_SIZE];
    uint8_t expected_mac[AES_BLOCK_SIZE] = {
        0xE1, 0x8D, 0x91, 0x79, 0xCB, 0xA8, 0x06, 0x08,
        0xC1, 0x0F, 0x0F, 0xF3, 0x7A, 0x64, 0xD9, 0x2E
    };
    
    print_hex("Key", key, AES_KEY_SIZE);
    print_hex("Message", message, message_len);
    
    aes_cmac(key, message, message_len, mac);
    
    print_hex("Computed CMAC", mac, AES_BLOCK_SIZE);
    print_hex("Expected CMAC", expected_mac, AES_BLOCK_SIZE);
    
    if (memcmp(mac, expected_mac, AES_BLOCK_SIZE) == 0) {
        printf("RESULT: PASSED!\n");
    } else {
        printf("RESULT: FAILED!\n");
        
        // Debug: Let's check intermediate values
        printf("\nDebug Information:\n");
        
        uint8_t k1[16], k2[16];
        uint8_t round_keys[176];
        uint8_t zero_block[16] = {0};
        uint8_t l[16];
        
        key_expansion(key, round_keys);
        aes_encrypt(zero_block, l, round_keys);
        print_hex("L (E_K(0))", l, 16);
        
        // Generate subkeys manually for debugging
        uint8_t const_Rb[16] = {0};
        const_Rb[15] = 0x87;
        
        debug_left_shift_one_bit(l, k1);
        if (l[0] & 0x80) {
            debug_xor_blocks(k1, const_Rb, k1);
        }
        print_hex("K1", k1, 16);
        
        debug_left_shift_one_bit(k1, k2);
        if (k1[0] & 0x80) {
            debug_xor_blocks(k2, const_Rb, k2);
        }
        print_hex("K2", k2, 16);
        
        // Test the subkey generation function
        uint8_t k1_test[16], k2_test[16];
        generate_subkeys_debug(key, k1_test, k2_test);
        print_hex("K1 (from function)", k1_test, 16);
        print_hex("K2 (from function)", k2_test, 16);
        
        // Test encryption of the message
        uint8_t encrypted[16];
        aes_encrypt(message, encrypted, round_keys);
        print_hex("E_K(M)", encrypted, 16);
    }
    printf("\n");
}


int main() {
    printf("AES128-CMAC Implementation Test\n");
    printf("================================\n\n");
    
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();
    
    return 0;
}