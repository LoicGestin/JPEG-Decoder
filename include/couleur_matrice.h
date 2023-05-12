#ifndef _COULEUR_MATRICE_H_
#define _COULEUR_MATRICE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "decode_entete.h"
uint8_t **YCbCr_to_R(uint8_t **Y, uint8_t **Cb, uint8_t **Cr);
uint8_t **YCbCr_to_B(uint8_t **Y, uint8_t **Cb, uint8_t **Cr);
uint8_t **YCbCr_to_G(uint8_t **Y, uint8_t **Cb, uint8_t **Cr);

#endif /* _COULEUR_MATRICE_H_ */