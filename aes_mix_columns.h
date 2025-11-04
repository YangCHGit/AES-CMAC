#ifndef AES_MIX_COLUMNS_H
#define AES_MIX_COLUMNS_H

#include "aes_types.h"

void mix_columns(state_t state);
void inv_mix_columns(state_t state);

#endif