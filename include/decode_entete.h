#ifndef _DECODE_ENTETE_H_
#define _DECODE_ENTETE_H_


typedef	unsigned char BYTE;

struct data;

struct dht_ac_dc;

struct component;

struct scan_component ;

struct data* init_data();

struct data* decode_entete(char * path);

#endif /* _DECODE_ENTETE_H_ */