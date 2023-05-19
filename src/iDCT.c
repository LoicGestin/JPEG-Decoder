#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/iDCT.h"

#define pi 3.14159265359



void iDCT_rapide(int16_t **phi, uint8_t **S ){
    /*Fonction réalisant l'inverse de la transformée en cosinus discrète à partir
    d'une matrice phi 8x8 et stockant le résultat dans une matrice S*/

    // dep = (1 / sqrt(16)) car n = 8
    float dep = (1 / sqrt(16));
    
    float racine = sqrt(2);
    float inverse_racine = 1 / racine;
    float cos6_16 = cosf((6 *pi)/16);
    float sin6_16 = sinf((6 *pi)/16);
    float cos3_16 = cosf((3 *pi)/16);
    float sin3_16 = sinf((3 *pi)/16);
    float cos1_16 = cosf((1 *pi)/16);
    float sin1_16 = sinf((1 *pi)/16);
    // Moitié du coefficient DC
    float t1 =  (float)phi[0][0] / 2;

    // sortirde iDCT
    // Pré calcule des cosinus pour réduire le nombre de calcul et rendre le code plus rapide
    // Création d'un tableau contenant tous les valuers des cosinus
   
    int16_t tmp[8][8];

      for(int8_t x=0; x<8; x++){
        
          
        float l0 = phi[x][0];
        float l4 = phi[x][1] - phi[x][7];
        float l2 = phi[x][2];
        float l6 = phi[x][5] * racine;
        float l7 = phi[x][1] + phi[x][7];
        float l3 = phi[x][6];
        float l5 = phi[x][3] * racine;
        float l1 = phi[x][4];

         
        float ll0 = l0 + l1;
        float ll1 = l0 - l1;
        float ll2 =  l2*racine*cos6_16 - l3* racine*sin6_16;
        float ll3 =  l3*racine*cos6_16 + l2* racine*sin6_16;
        float ll4 = l4 + l6;
        float ll5 = l7 - l5;
        float ll6 = l4 - l6;
        float ll7 = l7 + l5;
        
        
        l0 = ll0 + ll3;
        l1 = ll1 + ll2;
        l2 = ll1 - ll2;
        l3 = ll0 - ll3;
        l4 = ll4 * cos3_16 - ll7 * sin3_16;
        l5 = ll5 * cos1_16 - ll6 * sin1_16;
        l6 = ll6 * cos1_16 + ll5 * sin1_16;
        l7 = ll7 * cos3_16 + ll4 * sin3_16;

        tmp[x][0] = l0 + l7;
        tmp[x][1] = l1 + l6; 
        tmp[x][2] = l2 + l5; 
        tmp[x][3] = l3 + l4; 
        tmp[x][4] = l3 - l4;  
        tmp[x][5] = l2 - l5;  
        tmp[x][6] = l1 - l6;  
        tmp[x][7] = l0 - l7; 

        
        
     
    }
    for(int8_t x=0; x<8; x++){

           
        float l0 = tmp[0][x];
        float l4 = tmp[1][x] - tmp[7][x];
        float l2 = tmp[2][x];
        float l6 = tmp[5][x] * racine;
        float l7 = tmp[1][x] + tmp[7][x];
        float l3 = tmp[6][x];
        float l5 = tmp[3][x] * racine;
        float l1 = tmp[4][x];

         
        float ll0 = l0 + l1;
        float ll1 = l0 - l1;
        float ll2 =  l2*racine*cos6_16 - l3* racine*sin6_16;
        float ll3 =  l3*racine*cos6_16 + l2* racine*sin6_16;
        float ll4 = l4 + l6;
        float ll5 = l7 - l5;
        float ll6 = l4 - l6;
        float ll7 = l7 + l5;
        
        
        l0 = ll0 + ll3;
        l1 = ll1 + ll2;
        l2 = ll1 - ll2;
        l3 = ll0 - ll3;
        l4 = ll4 * cos3_16 - ll7 * sin3_16;
        l5 = ll5 * cos1_16 - ll6 * sin1_16;
        l6 = ll6 * cos1_16 + ll5 * sin1_16;
        l7 = ll7 * cos3_16 + ll4 * sin3_16;
 
        S[0][x] = (uint8_t)round(fmaxf(0, fminf(255, (l0 + l7)/8+128)));
        S[1][x] = (uint8_t)round(fmaxf(0, fminf(255, (l1 + l6)/8+128))); 
        S[2][x] = (uint8_t)round(fmaxf(0, fminf(255, (l2 + l5)/8+128))); 
        S[3][x] = (uint8_t)round(fmaxf(0, fminf(255, (l3 + l4)/8+128))); 
        S[4][x] = (uint8_t)round(fmaxf(0, fminf(255, (l3 - l4)/8+128)));  
        S[5][x] = (uint8_t)round(fmaxf(0, fminf(255, (l2 - l5)/8+128)));  
        S[6][x] = (uint8_t)round(fmaxf(0, fminf(255, (l1 - l6)/8+128)));  
        S[7][x] = (uint8_t)round(fmaxf(0, fminf(255, (l0 - l7)/8+128))); 
     
    }
}
