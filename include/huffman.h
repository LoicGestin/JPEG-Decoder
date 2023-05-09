#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <decode_entete.h>

void insert_code_huffman(struct cellule_huffman **c,int value, int length, BYTE symbol);

void display_huffman_tree(struct cellule_huffman* c, char* path);

void decode_huffman(struct data* d, int index, int table_type);


#endif /*_HUFFMAN_H_*/