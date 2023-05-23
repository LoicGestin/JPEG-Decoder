#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/couleur_matrice.h"
#include "../include/decode_entete.h"
void YCbCr_to_RGB(uint8_t **Y, uint8_t **Cb, uint8_t **Cr, struct data *d, uint8_t **R, uint8_t **G, uint8_t **B){
    // Récupération des facteurs d'échantillonnages
    struct component *comp = d->list_component;
    int8_t sampling_w = comp->sampling_horizontal;
    int8_t sampling_h = comp->sampling_vertical;
    
    // Remplissage de la matrice
    for(uint8_t i=0; i<8*sampling_h; i++){
        for(uint8_t j=0; j<8*sampling_w; j++){

            // Pré-calcul des valeurs Cb et Cr ajustées
            float Cb_adj = Cb[i][j] - 128;
            float Cr_adj = Cr[i][j] - 128;
            
            float r = Y[i][j] + 1.402 * Cr_adj;
            float b = Y[i][j] + 1.772 * Cb_adj;
            float g = Y[i][j] - 0.34414 * Cb_adj - 0.71414 * Cr_adj;

            // Troncature si les valeurs sont négatives ou supérieures à 255
            R[i][j] = r < 0 ?  0 : r > 255 ?  255 : (uint8_t) r;
            G[i][j] = g < 0 ?  0 : g > 255 ?  255 : (uint8_t) g;
            B[i][j] = b < 0 ?  0 : b > 255 ?  255 : (uint8_t) b;
        }
    }
}