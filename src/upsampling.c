#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/upsampling.h"


uint8_t ** sur_ech_h_Cb1(uint8_t **Cb){
    // initialisation nouvelle matrice
    uint8_t **new_Cb = malloc(8*sizeof(uint8_t*));

    for(int8_t i = 0; i<8; i++){
        new_Cb[i] = malloc(8*sizeof(uint8_t));
    }

    uint8_t *tab = malloc(32*sizeof(uint8_t));
    uint8_t *tab_dup = malloc(32*sizeof(uint8_t));

    int8_t k = 0;

    // Copie des valeurs dans un tableau
    while(k<32){
        for(int8_t i = 0; i < 4; i++){
            for(int8_t j = 0; j < 8; j++){
                tab[k] = Cb[i][j];
                tab_dup[k] = Cb[i][j];
                k++;
            }
        }
    }


    int8_t k_tab = 0;
    int8_t k_tab_dup = 0;

    for(int8_t i = 0; i < 8; i++){
        for(int8_t j = 0; j < 8; j++){
            if((i + j) % 2 == 0){
                new_Cb[i][j] = tab[k_tab];
                k_tab ++;
            }
            else{
                new_Cb[i][j] = tab[k_tab_dup];
                k_tab_dup ++;
            }
        }
    }

    return new_Cb;
}

uint8_t ** sur_ech_h_Cb2(uint8_t **Cb){
    // initialisation nouvelle matrice
    uint8_t **new_Cb = malloc(8*sizeof(uint8_t*));

    for(int8_t i = 0; i<8; i++){
        new_Cb[i] = malloc(8*sizeof(uint8_t));
    }

    uint8_t *tab = malloc(32*sizeof(uint8_t));
    uint8_t *tab_dup = malloc(32*sizeof(uint8_t));

    int8_t k = 0;

    // Copie des valeurs dans un tableau
    while(k<32){
        for(int8_t i = 4; i < 8; i++){
            for(int8_t j = 0; j < 8; j++){
                tab[k] = Cb[i][j];
                tab_dup[k] = Cb[i][j];
                k++;
            }
        }
    }


    int8_t k_tab = 0;
    int8_t k_tab_dup = 0;

    for(int8_t i = 0; i < 8; i++){
        for(int8_t j = 0; j < 8; j++){
            if((i + j) % 2 == 0){
                new_Cb[i][j] = tab[k_tab];
                k_tab ++;
            }
            else{
                new_Cb[i][j] = tab[k_tab_dup];
                k_tab_dup ++;
            }
        }
    }

    return new_Cb;
}

