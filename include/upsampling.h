#ifndef _UPSAMPLING_H_
#define _UPSAMPLING_H_

uint8_t ** sur_ech_h_Cb1(uint8_t **Cb);

uint8_t ** sur_ech_h_Cb2(uint8_t **Cb);

void sur_ech_v(uint8_t *Cb, uint8_t *Cr, int largeur, int hauteur);

#endif /*_UPSAMPLING_H_*/