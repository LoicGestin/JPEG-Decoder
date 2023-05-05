#include <stdlib.h>
#include <stdio.h>

#include "../include/decode_entete.h"

int * quantification_inverse(struct data *d, int index, int *frequentiel){
    int *quantification = d.quantization_table_read[index];
    int *table = malloc(64*sizeof(int*));
    for(int i=0; i<64; i++){
        table[i]= frequentiel[i]*quantification[i]; 
        }
    
    return table; 
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
    int *table=quantification_inverse(fréquentiel, quantification);
     for(int i = 0; i < 64; i ++){
        printf("%03d ",table[i]);
        }
        printf("\n");
    
    return 0;

}