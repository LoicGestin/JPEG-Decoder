#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/zig_zag.h"

void zig_zag(const int16_t *tab, int16_t **matrice){
/*Fonction prenant en entrée un pointeur vers un tableau de 64 valeurs et une matrice 8x8 et faisant
l'opération de zig zag inverse et plaçant le résultat dans matrice */

    
    int8_t i =0;
    int8_t j=0;
    int8_t k=0;

    // Iniatialisation du premier élément de la matrice 
    matrice[i][j]=tab[k];
    j++;
    k++;
    
    while (k<64){
        /* On divise la matrice en deux puisque dans la première moitié de celle-ci une remontée du zig-zag 
        dépend des lignes (i) et la descente du zig-zag des colonnes (j). Cet échange ce fait au 35 ème 
        terme de tab[] */

        if (k<35){
            // Ici on effectue une descente du zig-zag
            if(j%2 && i==0){
                while(j!=0){
                    matrice[i][j]=tab[k];
                    j--;
                    i++;
                    k++;
                }
                matrice[i][j]=tab[k];
                k++;
            }
            // Ici on effectue une remontée du zig-zag
            else if(i%2==0 && j==0){
                while(i!=0){
                    matrice[i][j]=tab[k];
                    i--;
                    j++;
                    k++;
                }
                matrice[i][j]=tab[k];
                k++;
            }
            // Ici on prépare la descente du zig-zag en ce décalant sur la colonne suivante
            else if(j%2==0 && i==0){
                j++;
            }
            // Ici on prépare la remontée du zig-zag en ce décalant sur la ligne suivante
            else if(i%2 && j==0){
                i++;
            }}

        else {
            // Ici on effectue une remontée du zig-zag
            if(j%2 && i==7){
                while(j!=7){
                    matrice[i][j]=tab[k];
                    j++;
                    i--;
                    k++;
                }
                matrice[i][j]=tab[k];
                k++;
            }
            // Ici on effectue une descente du zig-zag
            else if(i%2==0 && j==7){
                while(i!=7){
                    matrice[i][j]=tab[k];
                    i++;
                    j--;
                    k++;
                }
                matrice[i][j]=tab[k];
                k++;
            }
            // Ici on prépare la remontée du zig-zag en ce décalant sur la ligne suivante
            else if(j%2==0 && i==7){
                j++;
            }
            // Ici on prépare la descente du zig-zag en ce décalant sur la colonne suivante
            else if(i%2 && j==7){
                i++;
            }
        }
    }


}