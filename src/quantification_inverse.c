#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/decode_entete.h"
#include "../include/quantification_inverse.h"

void quantification_inverse(struct data *d, int8_t index,  int16_t *frequentiel){
    /*Fonction prenant en entrée un pointeur vers notre structure data, un index
    et un pointeur vers le bloc fréquentiel et effectuantla multiplication élément
    par élément de la table de quantification par le bloc fréquentiel*/
    
    int16_t *quantification = d->quantization_table_read[index];
    for(int8_t i=0; i<64; i++){
        frequentiel[i]= frequentiel[i]*quantification[i];
    }
}

