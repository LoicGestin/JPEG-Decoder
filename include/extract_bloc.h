#ifndef _EXTRACT_BLOC_H_
#define _EXTRACT_BLOC_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <decode_entete.h>

void init_blocks(struct data *d);

int read_bit(BYTE byte, int num_bit);

int16_t * decode_ac_dc(struct data *d, int index, int table_type,FILE* file,  int16_t * block);

#endif /* _EXTRACT_BLOC_H_ */