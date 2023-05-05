#ifndef _DECODE_ENTETE_H_
#define _DECODE_ENTETE_H_


typedef	unsigned char BYTE;

struct data;

struct dht_ac_dc;

struct component;

struct scan_component ;

struct data* init_data();

void decode_huffman(struct data* d, int index, int table_type);

char * decimal_to_binary(int val, int length);

struct data* decode_entete(char * path);

#endif /* _DECODE_ENTETE_H_ */