#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/couleur_matrice.h"
#include "../include/decode_entete.h"

void YCbCr_to_R(uint8_t **Y, uint8_t **Cr, struct data *d, uint8_t **R){

    struct component *comp = d->list_component;
    int8_t sampling_w = comp->sampling_horizontal;
    int8_t sampling_h = comp->sampling_vertical;
    
    for(uint8_t i=0; i<8*sampling_h; i++){
        for(uint8_t j=0; j<8*sampling_w; j++){
            //float r = Y[i][j] - 0.0009267 * (Cb[i][j] - 128) + 1.4016868 * (Cr[i][j] - 128);
            float r = Y[i][j] + 1.402 * (Cr[i][j] - 128);
            if(r < 0){
                R[i][j] = 0;
            }
            else if(r > 255){
                R[i][j] = 255;
            }
            else{
                R[i][j] = (uint8_t) r;
            }

        }
    }
    
}

void YCbCr_to_B(uint8_t **Y, uint8_t **Cb, struct data *d, uint8_t **B){

    struct component *comp = d->list_component;
    int8_t sampling_w = comp->sampling_horizontal;
    int8_t sampling_h = comp->sampling_vertical;
    
    for(uint8_t i=0; i<8*sampling_h; i++){
        for(uint8_t j=0; j<8*sampling_w; j++){
            //float b  = Y[i][j] + 1.7721604 * (Cb[i][j] - 128) + 0.0009902 * (Cr[i][j] - 128);
            float b = Y[i][j] + 1.772 * (Cb[i][j] - 128);
            if(b < 0){
                B[i][j] = 0;
            }
            else if(b > 255){
                B[i][j] = 255;
            }
            else{
                B[i][j] = (uint8_t) b;
            }
            
        }
    }
    
}

void YCbCr_to_G(uint8_t **Y, uint8_t **Cb, uint8_t **Cr, struct data *d, uint8_t **G){

    struct component *comp = d->list_component;
    int8_t sampling_w = comp->sampling_horizontal;
    int8_t sampling_h = comp->sampling_vertical;
    
    for(uint8_t i=0; i<8*sampling_h; i++){
        for(uint8_t j=0; j<8*sampling_w; j++){
           //float g = Y[i][j] - 0.3436954 * (Cb[i][j] - 128) - 0.7141690 * (Cr[i][j] - 128);
           float g = Y[i][j] - 0.34414 * (Cb[i][j] - 128) - 0.71414 * (Cr[i][j] - 128);

           if(g < 0){
                G[i][j] = 0;
            }
            else if(g > 255){
                G[i][j] = 255;
            }
            else{
                G[i][j] = (uint8_t)g;
            }
        }
    }
   
}
