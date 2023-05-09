#include <stdlib.h>
#include <stdio.h>

#include "../include/zig_zag.h"

int16_t ** zig_zag(const int16_t *tab){


    int16_t **matrice = malloc(8*sizeof(int16_t*));
    for(int8_t i=0; i<8; i++){
        matrice[i]=malloc(8*sizeof(int16_t));
    }
    int8_t i =0;
    int8_t j=0;
    int8_t k=0;
    matrice[i][j]=tab[k];
    j++;
    k++;
    while (k<64){
        if (k<35){
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
            else if(j%2==0 && i==0){
                j++;
            }
            else if(i%2 && j==0){
                i++;
            }}

        else {
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
            else if(j%2==0 && i==7){
                j++;
            }
            else if(i%2 && j==7){
                i++;
            }
        }
    }


    return matrice;
}