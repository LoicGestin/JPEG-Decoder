#ifndef _COULEUR_MATRICE_H_
#define _COULEUR_MATRICE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "decode_entete.h"

void YCbCr_to_RGB(uint8_t **Y, uint8_t **Cb, uint8_t **Cr, struct data *d, uint8_t **R, uint8_t **G, uint8_t **B);

#endif /* _COULEUR_MATRICE_H_ */
