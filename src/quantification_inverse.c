#include <stdlib.h>
#include <stdio.h>

#include "../include/decode_entete.h"
#include "../include/quantification_inverse.h"

int * quantification_inverse(struct data *d, int index,  const int16_t *frequentiel){
    int *quantification = d->quantization_table_read[index];
    int *table = malloc(64*sizeof(int*));
    for(int i=0; i<64; i++){
        table[i]= frequentiel[i]*quantification[i];
    }

    return table;
}

