#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include "decode_entete.h"

void insert_code_huffman(struct cellule_huffman **c,int16_t value, int16_t length, BYTE symbol);

void decode_huffman(struct data* d, int16_t index, int8_t table_type);


#endif /*_HUFFMAN_H_*/