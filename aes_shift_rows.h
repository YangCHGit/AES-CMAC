#ifndef AES_SHIFT_ROWS_H
#define AES_SHIFT_ROWS_H

#include "aes_types.h"

void shift_rows(state_t state);
void inv_shift_rows(state_t state);

#endif