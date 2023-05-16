#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include "decode_entete.h"

void insert_code_huffman(struct cellule_huffman **c,int16_t value, int16_t length, int16_t symbol);

void decode_huffman(struct dht_ac_dc *current_dht, int16_t index, int8_t table_type);

void display_huffman_tree(struct cellule_huffman* c, char* path);

void free_huff(struct cellule_huffman *h);

#endif /*_HUFFMAN_H_*/