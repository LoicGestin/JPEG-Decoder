#ifndef _IDCT_H_
#define _IDCT_H_
#include <stdint.h>

void iDCT_rapide(int16_t **phi, uint8_t **S );

void iDCT(int16_t **phi, uint8_t **S);

#endif /*_IDCT_H_*/
