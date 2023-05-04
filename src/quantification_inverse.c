#include <stdlib.h>
#include <stdio.h>

int ** quantification_inverse(int **frequentiel, int *quantification){

    int **matrice = malloc(8*sizeof(int*));
    for(int i=0; i<8; i++){
        matrice[i]=malloc(8*sizeof(int));
    }
    int k=0;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            matrice[i][j]= frequentiel[i][j]*quantification[k];
            k++;  
        }
    }
    
    return matrice; 
}

int   main(){
    int **fréquentiel = malloc(8*sizeof(int*));
    for(int i=0; i<8; i++){
        fréquentiel[i]=malloc(8*sizeof(int));
    }
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            fréquentiel[i][j]=2;
        }
    }
    int *quantification = malloc(64*sizeof(int));
    for(int i=0; i<64; i++){
        quantification[i]=i;
    }
    int **matrice=quantification_inverse(fréquentiel, quantification);
     for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            printf("%04d ",matrice[i][j]);
        }
        printf("\n");
    }
    return 0;

}