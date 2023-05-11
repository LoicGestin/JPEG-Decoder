#ifndef _PPM_H_
#define _PPM_H_

#include <stdint.h>

void create_pgm_header(FILE *file_name, int16_t width, int16_t height);

void create_pgm(FILE *file_name, uint8_t ***nuance, int16_t width, int16_t height);

void create_ppm_header(FILE *file_name, int16_t width, int16_t height);

void create_ppm(FILE *file_name, const uint8_t ** R, const uint8_t ** G, const uint8_t ** B, int16_t width, int16_t height)

#endif /* _PPM_H_*/