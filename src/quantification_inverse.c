#include <stdlib.h>
#include <stdio.h>

#include "../include/decode_entete.h"
#include "../include/quantification_inverse.h"

int16_t * quantification_inverse(struct data *d, int8_t index,  const int16_t *frequentiel){
    /*Fonction prenant en entrée un pointeur vers notre structure data, un index
    et un pointeur vers et un pointeur vers le bloc fréquentiel et effectuant
    la multiplication élément par élément de la table de quantification par le
    bloc fréquentiel. On retournera le tableau obtenu*/
    int16_t *quantification = d->quantization_table_read[index];
    int16_t *table = malloc(64*sizeof(int*));
    for(int8_t i=0; i<64; i++){
        table[i]= frequentiel[i]*quantification[i];
    }

    return table;
}

