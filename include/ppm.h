#ifndef _PPM_H_
#define _PPM_H_

#include <stdint.h>

void create_ppm(char * file_name, const uint8_t **R, const uint8_t ** G, const uint8_t ** B, int8_t width, int8_t height);

void create_pgm(char *file_name, uint8_t **nuance, int8_t width, int8_t height);

#endif /* _PPM_H_*/