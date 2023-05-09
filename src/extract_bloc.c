#include <stdio.h>
#include<stdlib.h>

#include "../include/extract_bloc.h"

void init_blocks(struct data *d) {
    int nb_block = d->image_height * d->image_width;
    d->decoded_blocks = malloc( sizeof(int ) * nb_block );
    for (int i = 0; i < nb_block; i++) {
        d->decoded_blocks[i] = malloc(sizeof(int ) * 64);
    }
}
int read_bit(BYTE byte, int num_bit){
    //printf("byte %x  : numéro %d = %d\n", byte,num_bit,(byte >> (num_bit )) & 1);
    return (byte >> ( 7 - num_bit )) & 1 ;
}
int16_t * decode_ac_dc(struct data *d, int index, int table_type,FILE* file,  int16_t * block){
    struct dht_ac_dc *current_dht = table_type ?&d->list_dc[index] : &d->list_ac[index];


    if(d->num_bit == -1) {
        fread(&d->byte, 1, 1, file);
        d->num_bit = 0;
    }
    //printf("byte = %x\n", d->byte);

    int bit;


    struct cellule_huffman *current_cel = current_dht->racine_huffman;

    int cpt =table_type ? 0 : 1;
    int val = table_type ? 1 : 64;

    while(cpt < val){
        // JE SUIS REST2 BLOQU2 2 PUTAIN DHEURE CAR J4AVAIS MIS UN && !!!!!!!!!!!!!!!!!!!!!!!!
        while(current_cel->right != NULL || current_cel->left != NULL){
            bit = read_bit(d->byte,d->num_bit);
            current_cel = bit ? current_cel->right : current_cel->left;
            if(d->num_bit == 7){
                fread(&d->byte, 1, 1, file);
                d->num_bit =0;
            }
            else{
                d->num_bit ++;
            }
        }
        if(current_cel->symbol == 0xff){
            printf("ERORR");
        }
        int8_t symbol =(int8_t) current_cel->symbol;

        if(symbol == 0){
            while(cpt != 64){

                block[cpt] = 0;
                cpt ++;
            }
        }
        int8_t magnitude = (int8_t) (symbol & 0xF);
        int8_t num_zeros = (int8_t) (symbol >> 4);
        while (num_zeros > 0) {

            block[cpt] = 0;
            num_zeros--;
            cpt ++;
        }
        int16_t value = 0;

        int8_t current_magnitude =  magnitude;
        if(current_magnitude > 0) {
            while (current_magnitude > 0) {
                bit = read_bit(d->byte, d->num_bit);
                value = (int16_t) ((value << 1) + bit);

                d->num_bit++;
                if (d->num_bit == 8) {
                    fread(&d->byte, 1, 1, file);
                    d->num_bit = 0;
                }
                current_magnitude--;
            }

            if ((value & (1 << (magnitude - 1))) == 0) {
                value +=(int16_t) (-(1 << magnitude) + 1);
            }

            block[cpt] = value;
            cpt ++;
        }
        else{

            block[cpt] =0;
            cpt ++;
        }
        printf("symbole %x, magnitude = %x, value = %x , num_zero %d, cpt = %d\n",symbol,magnitude,value,symbol >> 4,cpt );
        current_cel = current_dht->racine_huffman;
    }
    for(int i =0; i < 64; i++){
        printf("%hx ", block[i]);
    }
    return block;
}