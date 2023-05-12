#include <stdlib.h>
#include <stdio.h>

#include "../include/couleur_matrice.h"

uint8_t **YCbCr_to_R(uint8_t **Y, uint8_t **Cb, uint8_t **Cr){
    uint8_t **R=malloc(8*sizeof(uint8_t *));
    for(uint8_t i=0; i<8; i++){
        R[i]=malloc(8*sizeof(uint8_t));
    }
    for(uint8_t i=0; i<8; i++){
        for(uint8_t j=0; j<8; j++){
            R[i][j]= Y[i][j] - 0.0009267 * (Cb[i][j] - 128) + 1.4016868 * (Cr[i][j] - 128);
        }
    }
    return R;
}

uint8_t **YCbCr_to_B(uint8_t **Y, uint8_t **Cb, uint8_t **Cr){
    uint8_t **B=malloc(8*sizeof(uint8_t *));
    for(uint8_t i=0; i<8; i++){
        B[i]=malloc(8*sizeof(uint8_t));
    }
    for(uint8_t i=0; i<8; i++){
        for(uint8_t j=0; j<8; j++){
            B[i][j]= Y[i][j] - 1.7721604 * (Cb[i][j] - 128) + 0.0009902 * (Cr[i][j] - 128);
        }
    }
    return B;
}

uint8_t **YCbCr_to_G(uint8_t **Y, uint8_t **Cb, uint8_t **Cr){
    uint8_t **G=malloc(8*sizeof(uint8_t *));
    for(uint8_t i=0; i<8; i++){
        G[i]=malloc(8*sizeof(uint8_t));
    }
    for(uint8_t i=0; i<8; i++){
        for(uint8_t j=0; j<8; j++){
           G[i][j]= Y[i][j] - 0.3436954 * (Cb[i][j] - 128) + 0.7141690 * (Cr[i][j] - 128);
        }
    }
    return G;
}