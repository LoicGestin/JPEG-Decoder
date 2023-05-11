#ifndef _COULEUR_H_
#define _COULEUR_H_

#include <stdint.h>

struct Tuple {
    uint8_t ** R, G, B;
};


struct Tuple YCbCr_to_RGB(uint8_t **Y, uint8_t **Cb, uint8_t **Cr);

#endif /*_COULEUR_H_*/