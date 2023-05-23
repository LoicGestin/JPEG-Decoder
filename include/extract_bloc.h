#ifndef _EXTRACT_BLOC_H_
#define _EXTRACT_BLOC_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decode_entete.h"

int8_t read_bit(BYTE byte, int8_t num_bit);

void decode_ac_dc(struct data *d, int16_t index, int8_t table_type,FILE* file,  int16_t * block);

#endif /* _EXTRACT_BLOC_H_ */