#ifndef _IDCT_H_
#define _IDCT_H_
#include <stdint.h>

void iDCT(int16_t **phi, uint8_t **S, float cos_tab[8][8]);

#endif /*_IDCT_H_*/
