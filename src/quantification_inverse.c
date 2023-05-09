#include <stdlib.h>
#include <stdio.h>

#include "../include/decode_entete.h"
#include "../include/quantification_inverse.h"

int16_t * quantification_inverse(struct data *d, int8_t index,  const int16_t *frequentiel){
    int16_t *quantification = d->quantization_table_read[index];
    int16_t *table = malloc(64*sizeof(int*));
    for(int8_t i=0; i<64; i++){
        table[i]= frequentiel[i]*quantification[i];
    }

    return table;
}

