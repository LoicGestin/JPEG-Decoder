#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <decode_entete.h>

void decode_huffman(struct data* d, int index, int table_type);

char * decimal_to_binary(int val, int length);


#endif /*_HUFFMAN_H_*/