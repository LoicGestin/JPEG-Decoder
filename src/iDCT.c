#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/iDCT.h"

#define pi 3.14159265359
#define racine  sqrt(2)
#define cos6_16  cosf((6 *pi)/16)
#define sin6_16  sinf((6 *pi)/16)
#define cos3_16  cosf((3 *pi)/16)
#define sin3_16  sinf((3 *pi)/16)
#define cos1_16  cosf((1 *pi)/16)
#define sin1_16  sinf((1 *pi)/16)


void iDCT_rapide(int16_t **phi, uint8_t **S ){
    /*Fonction réalisant l'inverse de la transformée en cosinus discrète à partir
    d'une matrice phi 8x8 et stockant le résultat dans une matrice S*/
   
    double tmp[8][8];

      for(int8_t x=0; x<8; x++){
        
          
        double l0 = phi[x][0];
        double l4 = phi[x][1] - phi[x][7];
        double l2 = phi[x][2];
        double l6 = phi[x][5] * racine;
        double l7 = phi[x][1] + phi[x][7];
        double l3 = phi[x][6];
        double l5 = phi[x][3] * racine;
        double l1 = phi[x][4];

         
        double ll0 = l0 + l1;
        double ll1 = l0 - l1;
        double ll2 =  l2*(racine*cos6_16) - l3* (racine*sin6_16);
        double ll3 =  l3*racine*cos6_16 + l2* racine*sin6_16;
        double ll4 = l4 + l6;
        double ll5 = l7 - l5;
        double ll6 = l4 - l6;
        double ll7 = l7 + l5;
        
        
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

           
        double l0 = tmp[0][x];
        double l4 = tmp[1][x] - tmp[7][x];
        double l2 = tmp[2][x];
        double l6 = tmp[5][x] * racine;
        double l7 = tmp[1][x] + tmp[7][x];
        double l3 = tmp[6][x];
        double l5 = tmp[3][x] * racine;
        double l1 = tmp[4][x];

         
        double ll0 = l0 + l1;
        double ll1 = l0 - l1;
        double ll2 =  l2*racine*cos6_16 - l3* racine*sin6_16;
        double ll3 =  l3*racine*cos6_16 + l2* racine*sin6_16;
        double ll4 = l4 + l6;
        double ll5 = l7 - l5;
        double ll6 = l4 - l6;
        double ll7 = l7 + l5;
        
        
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


void iDCT(int16_t **phi, uint8_t **S){
    /*Fonction réalisant l'inverse de la transformée en cosinus discrète à partir
    d'une matrice phi 8x8 et stockant le résultat dans une matrice S*/

    float cos_table[8][8];
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            cos_table[i][j] = cosf(((2*j+1)*i*pi)/(16));
        }
    }

    // dep = (1 / sqrt(16)) car n = 8
    float dep = (1 / sqrt(16));

    // Moitié du coefficient DC
    float t1 =  (float)phi[0][0] / 2;

    // Parcours des éléments de la matrice S
    for(int8_t x=0; x<8; x++){

        for(int8_t y=0; y<8; y++){

            // Calcul de t2 grâce à une somme sur mu
            float t2 = 0.0;
            for (int8_t mu = 1; mu<8; mu++){
                t2 += (float) (cos_table[mu][y] * (float) phi[0][mu])/racine;
            }

            // Calcul de t3 en utilisant une somme sur lambda
            float t3 = 0.0;
            for (int lambda = 1; lambda<8; lambda++){
                t3 += ((float) cos_table[lambda][x] *  (float)phi[lambda][0])/racine;
            }

            // Calcul de t4 en effectuant une double somme sur lambda et mu
            float t4 = 0.0;
            for (int8_t lambda = 1; lambda<8; lambda++){
                for (int8_t mu = 1; mu<8; mu++){
                    t4 += (float) cos_table[mu][y] * cos_table[lambda][x] *  (float)phi[lambda][mu];
                }
            }
      
            // Regroupement des termes et stockage des valeurs
            float s = dep * (t1 + t2 + t3 + t4);
            S[x][y] = (uint8_t)round(fmaxf(0, fminf(255, s + 128)));
           
        }
    } 
}
