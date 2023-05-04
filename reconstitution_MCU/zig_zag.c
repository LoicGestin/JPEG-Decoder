#include <stdlib.h>
#include <stdio.h>
int ** zig_zag(int *tab){


    int **matrice = malloc(8*sizeof(int*));
    for(int i=0; i<8; i++){
        matrice[i]=malloc(8*sizeof(int));
    }
    int i =0;
    int j=0;
    int k=0;
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

int main(){
    int *tab=malloc(64*sizeof(int));
    for(int i=0; i<64; i++){
        tab[i]=i;
    }
    int **matrice=zig_zag(tab);
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            printf("%02d ",matrice[i][j]);
        }
        printf("\n");
    }
    return 0;
}



void affiche_matrice(int **matrice){
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            printf("%02d ",matrice[i][j]);
        }
        printf("\n");
    }
}