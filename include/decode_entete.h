#ifndef _DECODE_ENTETE_H_
#define _DECODE_ENTETE_H_


typedef	unsigned char BYTE;



struct data* init_data();

struct data* decode_entete(char * path);

#endif /* _DECODE_ENTETE_H_ */