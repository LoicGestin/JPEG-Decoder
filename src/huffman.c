#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void decode_huffman(struct data* d, int index, int table_type){
    struct dht_ac_dc *current_dht = table_type ? &d->list_ac[index] :  &d->list_dc[index];
    current_dht->huff_code = malloc(current_dht->nb_symbols * sizeof(int));
    current_dht->huff_length = malloc(current_dht->nb_symbols * sizeof(int));

    int ind = 0;
    int value = 0;
    int length = 1;
    for(int i=0; i<16; i ++){

        for(int j=0; j < current_dht->nb_code[i]; j++){
            current_dht->huff_code[ind] = value;
            current_dht->huff_length[ind] = length;
            value ++;
            ind ++;
        }
        value <<=1;
        length ++;
    }

}
char * decimal_to_binary(int val, int length){
    char *res = malloc(length + 1);
    res[length] = '\0';

    for (int i = length - 1; i >= 0; i--) {

        res[i] = (val & 1) + '0';
        val >>= 1;
    }

    return res;
}
struct arbre_binaire{
    struct arbre_binaire *parent;
    struct arbre_binaire *fils_droit;
    struct arbre_binaire *fils_gauche;
    int *clé;
};

struct arbre_binaire arbre_huffman(struct dht_ac_dc *huff){
    
}


int main(int argc, char **argv)
{
    /*
    if (argc != 2) {

          #  Si y'a pas au moins un argument en ligne de commandes, on
          #  boude.

        fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);

        return EXIT_FAILURE;
    }
    */
    //struct data *d = decode_entete("C:/Users/loicg/CLionProjects/team19/images/invader.jpeg");
    /* On se congratule. */
    return EXIT_SUCCESS;
}