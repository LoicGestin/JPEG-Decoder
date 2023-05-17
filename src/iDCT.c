#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/iDCT.h"



void iDCT(int16_t **phi, uint8_t **S, float cos_tab[8][8]){
    /*Fonction réalisant l'inverse de la transformée en cosinus discrète à partir
    d'une matrice phi 8x8 et stockant le résultat dans une matrice S*/

    // dep = (1 / sqrt(16)) car n = 8
    float dep = (1 / sqrt(16));
    
    float racine = sqrt(2);

    // Moitié du coefficient DC
    float t1 =  (float)phi[0][0] / 2;

    // sortirde iDCT
    // Pré calcule des cosinus pour réduire le nombre de calcul et rendre le code plus rapide
    // Création d'un tableau contenant tous les valuers des cosinus
   


    // Parcours des éléments de la matrice S
    for(int8_t x=0; x<8; x++){

        for(int8_t y=0; y<8; y++){
            
            // Calcul de t2 grâce à une somme sur mu
            float t2 = 0.0;
            for (int8_t mu = 1; mu<8; mu++){
                t2 += (float) (cos_tab[mu][y] * (float) phi[0][mu])/racine;
            }

            // Calcul de t3 en utilisant une somme sur lambda
            float t3 = 0.0;
            for (int lambda = 1; lambda<8; lambda++){
                t3 += (float) (cos_tab[lambda][x] *  (float)phi[lambda][0])/racine;
            }

            // Calcul de t4 en effectuant une double somme sur lambda et mu
            float t4 = 0.0;
            for (int8_t lambda = 1; lambda<8; lambda++){
                for (int8_t mu = 1; mu<8; mu++){
                    t4 += (float) (cos_tab[mu][y] * cos_tab[lambda][x] *  (float)phi[lambda][mu]);
                }
            }
      
            // Regroupement des termes et stockage des valeurs
            float s = dep * (t1 + t2 + t3 + t4);
            S[x][y] = (uint8_t)round(fmaxf(0, fminf(255, s + 128)));
           
        }
    }
}
