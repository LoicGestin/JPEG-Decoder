#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/upsampling.h"
#include "../include/decode_entete.h"


void sur_ech(uint8_t **Cb, struct data *d, uint8_t **new_Cb){
    // initialisation nouvelle matrice
    struct component *comp = d->list_component;
    int8_t sampling_w = comp->sampling_horizontal;
    int8_t sampling_h = comp->sampling_vertical;
    

    for(int16_t i = 0; i<8; i++){
        for(int x = 0; x < sampling_h; x ++ ){
            for(int j=0; j < 8; j ++){    
                for(int y = 0; y < sampling_w; y ++){
                    new_Cb[(i * sampling_h) + x][(j*sampling_w) + y] = Cb[i][j]; 
                }
            }  
        }
    }
}

