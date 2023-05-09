#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/upsampling.h"


// nb bloc horizontaux multpliés par 2
void sur_ech_h(uint8_t *Cb, uint8_t *Cr, int largeur, int hauteur){
    // nouvelles matrices Cb et Cr
    uint8_t *new_Cb = malloc((largeur*2)*hauteur*sizeof(uint8_t));
    uint8_t *new_Cr = malloc((largeur*2)*hauteur*sizeof(uint8_t));
    
    // copie des valeurs de chrominances
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            new_Cb[2*x + y*(largeur*2)] = Cb[x + y*largeur];
            new_Cr[2*x + y*(largeur*2)] = Cr[x + y*largeur];
            
            // calcul de la moyenne entre un pixel et le précendant et on place la valeur moyenne dans la nouvelle matrice
            if (x > 0) {
                new_Cb[2*x - 1 + y*(largeur*2)] = (Cb[x - 1 + y*largeur] + Cb[x + y*largeur]) / 2;
                new_Cr[2*x - 1 + y*(largeur*2)] = (Cr[x - 1 + y*largeur] + Cr[x + y*largeur]) / 2;
            }
        }
    }
    
    // copie des valeurs des nouvelles matrices dans les anciennes
    memcpy(Cb, new_Cb, (largeur*2)*hauteur*sizeof(uint8_t));
    memcpy(Cr, new_Cr, (largeur*2)*hauteur*sizeof(uint8_t));
    
    // libération de la mémoire
    free(new_Cb);
    free(new_Cr);
}

// nb bloc verticaux multpliés par 2
void sur_ech_v(uint8_t *Cb, uint8_t *Cr, int largeur, int hauteur){
    // nouvelles matrices Cb et Cr
    uint8_t *new_Cb = malloc(largeur*(hauteur*2)*sizeof(uint8_t));
    uint8_t *new_Cr = malloc(largeur*(hauteur*2)*sizeof(uint8_t));
    
    // copie des valeurs de chrominances
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            new_Cb[x + 2*y*largeur] = Cb[x + y*largeur];
            new_Cr[x + 2*y*largeur] = Cr[x + y*largeur];
            
            // calcul de la moyenne entre un pixel et le précendant et on place la valeur moyenne dans la nouvelle matrice
            if (y > 0) {
                new_Cb[x + (2*y - 1)*largeur] = (Cb[x + (y - 1)*largeur] + Cb[x + y*largeur]) / 2;
                new_Cr[x + (2*y - 1)*largeur] = (Cr[x + (y - 1)*largeur] + Cr[x + y*largeur]) / 2;
            }
        }
    }
    
    // copie des valeurs des nouvelles matrices dans les anciennes
    memcpy(Cb, new_Cb, largeur*(hauteur*2)*sizeof(uint8_t));
    memcpy(Cr, new_Cr, largeur*(hauteur*2)*sizeof(uint8_t));
    
    // libération de la mémoire
    free(new_Cb);
    free(new_Cr);
}


int main() {
    // définition de matrices pour les tests
    uint8_t Cb[4][4] = {{10, 20, 30, 40},
                              {50, 60, 70, 80},
                              {90, 100, 110, 120},
                              {130, 140, 150, 160}};
                              
    uint8_t Cr[4][4] = {{200, 180, 160, 140},
                              {120, 100, 80, 60},
                              {40, 20, 0, 255},
                              {200, 180, 160, 140}};
                              
    // matrices d'entrées
    printf("entrées:\n");
    printf("Cb:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", Cb[i][j]);
        }
        printf("\n");
    }
    printf("\nCr:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", Cr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    // utilisation du sur echantillonnage horizontal
    sur_ech_v((uint8_t *)Cb, (uint8_t *)Cr, 4, 4);
    
    // les matrices de sorties
    printf("sorties:\n");
    printf("Cb:\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", Cb[i][j]);
        }
        printf("\n");
    }
    printf("\nCr:\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", Cr[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}

