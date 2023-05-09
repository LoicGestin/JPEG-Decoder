#ifndef _EXTRACT_BLOC_H_
#define _EXTRACT_BLOC_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decode_entete.h"

void init_blocks(struct data *d);

int8_t read_bit(BYTE byte, int8_t num_bit);

int16_t * decode_ac_dc(struct data *d, int8_t index, int8_t table_type,FILE* file,  int16_t * block);

#endif /* _EXTRACT_BLOC_H_ */