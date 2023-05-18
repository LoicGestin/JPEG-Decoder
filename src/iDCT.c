#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/iDCT.h"

#define pi 3.14159265359

void iDCT(int16_t **phi, uint8_t **S, float cos_tab[8][8]){
    /*Fonction réalisant l'inverse de la transformée en cosinus discrète à partir
    d'une matrice phi 8x8 et stockant le résultat dans une matrice S*/

    // dep = (1 / sqrt(16)) car n = 8
    float dep = (1 / sqrt(16));
    
    float racine = sqrt(2);
    float inverse_racine = 1 / racine;
    float cos_16 = cosf(pi/16);
    float sin_16 = sinf(pi/16);
    // Moitié du coefficient DC
    float t1 =  (float)phi[0][0] / 2;

    // sortirde iDCT
    // Pré calcule des cosinus pour réduire le nombre de calcul et rendre le code plus rapide
    // Création d'un tableau contenant tous les valuers des cosinus
   
    int16_t tmp[8][8];

    // Parcours des éléments de la matrice S
      for(int8_t x=0; x<8; x++){

        float l0 = (phi[x][0]+ phi[x][4])/2;
        float l4 = (phi[x][0]- phi[x][4])/2;
        float l2 = phi[x][2]*inverse_racine*cos_16 - phi[x][6] * inverse_racine*sin_16;
        float l6 = phi[x][6]*inverse_racine*cos_16 + phi[x][2] * inverse_racine*sin_16;
        float l7 = ((phi[x][1] - phi[x][7])/2 + (phi[x][5]*inverse_racine))/2;
        float l1 = ((phi[x][1] + phi[x][7])/2 - inverse_racine*phi[x][3])/2;
        float l3 = ((phi[x][1] - phi[x][7])/2 - (phi[x][5]*inverse_racine))/2;
        float l5 = (inverse_racine*phi[x][3] + (phi[x][1] + phi[x][7])/2)/2;

        float ll0 = (l0 + l6)/2;
        float ll4 = (l4 + l2)/2;
        float ll2 = (l4 - l2)/2;
        float ll6 = (l0 - l6)/2;
        float ll7 = l7*cos_16 - l1*sin_16;
        float ll1 = l1*cos_16 + l7*sin_16;
        float ll3 = l3*cos_16 - l5*sin_16;
        float ll5 = l5*cos_16 - l3*sin_16;

    
        tmp[x][0] = ll0 + ll1;
        tmp[x][4] = ll4 + ll5; 
        tmp[x][2] = ll2 + ll3; 
        tmp[x][6] = ll6 + ll7; 
        tmp[x][7] = ll6 - ll7;  
        tmp[x][1] = ll0 - ll1;  
        tmp[x][3] = ll2 - ll3;  
        tmp[x][5] = ll4 - ll5; 
     
    }
    for(int8_t x=0; x<8; x++){

        float l0 = (tmp[0][x]+ tmp[4][x])/2;
        float l4 = (tmp[0][x]- tmp[4][x])/2;
        float l2 = tmp[2][x]*inverse_racine*cos_16 - tmp[6][x]* inverse_racine*sin_16;
        float l6 = tmp[6][x]*inverse_racine*cos_16 + tmp[2][x] * inverse_racine*sin_16;
        float l7 = ((tmp[1][x] - tmp[7][x])/2 + (tmp[5][x]*inverse_racine))/2;
        float l1 = ((tmp[1][x] + tmp[7][x])/2 - inverse_racine*tmp[3][x])/2;
        float l3 = ((tmp[1][x] - tmp[7][x])/2 - (tmp[5][x]*inverse_racine))/2;
        float l5 = (inverse_racine*tmp[3][x] + (tmp[1][x] + tmp[7][x])/2)/2;

        float ll0 = (l0 + l6)/2;
        float ll4 = (l4 + l2)/2;
        float ll2 = (l4 - l2)/2;
        float ll6 = (l0 - l6)/2;
        float ll7 = l7*cos_16 - l1*sin_16;
        float ll1 = l1*cos_16 + l7*sin_16;
        float ll3 = l3*cos_16 - l5*sin_16;
        float ll5 = l5*cos_16 - l3*sin_16;

    
        S[0][x] = (uint8_t)round(fmaxf(0, fminf(255, (ll0 + ll1)*2+128)));
        S[4][x] = (uint8_t)round(fmaxf(0, fminf(255, (ll4 + ll5)*2+128))); 
        S[2][x] = (uint8_t)round(fmaxf(0, fminf(255, (ll2 + ll3)*2+128))); 
        S[6][x] = (uint8_t)round(fmaxf(0, fminf(255, (ll6 + ll7)*2+128))); 
        S[7][x] = (uint8_t)round(fmaxf(0, fminf(255, (ll6 - ll7)*2+128)));  
        S[1][x] = (uint8_t)round(fmaxf(0, fminf(255, (ll0 - ll1)*2+128)));  
        S[3][x] = (uint8_t)round(fmaxf(0, fminf(255, (ll2 - ll3)*2+128)));  
        S[5][x] = (uint8_t)round(fmaxf(0, fminf(255, (ll4 - ll5)*2+128))); 
     
    }
}
