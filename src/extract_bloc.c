#include <stdio.h>
#include<stdlib.h>

#include "../include/extract_bloc.h"


int8_t read_bit(BYTE byte, int8_t num_bit){
    //printf("byte %x  : numéro %d = %d\n", byte,num_bit,(byte >> (num_bit )) & 1);
    return (byte >> ( 7 - num_bit )) & 1 ;
}
int16_t * decode_ac_dc(struct data *d, int16_t index, int8_t table_type,FILE* file,  int16_t * block){
    struct dht_ac_dc *current_dht = table_type ?&d->list_dc[index] : &d->list_ac[index];
   
    if(d->num_bit == -1) {
        fread(&d->byte, 1, 1, file);
        d->num_bit = 0;
    }
    //printf("byte = %x\n", d->byte);

    int8_t bit;


    struct cellule_huffman *current_cel = current_dht->racine_huffman;

    int8_t cpt =table_type ? 0 : 1;
    int8_t val = table_type ? 1 : 64;

    while(cpt < val){
        // JE SUIS REST2 BLOQU2 2 PUTAIN DHEURE CAR J4AVAIS MIS UN && !!!!!!!!!!!!!!!!!!!!!!!!
        while(current_cel->right != NULL || current_cel->left != NULL){
            bit = read_bit(d->byte,d->num_bit);
            //printf("Byte = %x  : %d %d %d %d %d %d %d %d\n",d->byte,read_bit(d->byte,0),read_bit(d->byte,1),read_bit(d->byte,2),read_bit(d->byte,3),read_bit(d->byte,4),read_bit(d->byte,5),read_bit(d->byte,6),read_bit(d->byte,7));
            current_cel = bit ? current_cel->right : current_cel->left;
            if(d->num_bit == 7){
                fread(&d->byte, 1, 1, file);
                if(d->find_ff){
                    fread(&d->byte, 1, 1, file);
                    d->find_ff = 0;
                }
                if(d->byte == 0xFF){
                    d->find_ff = 1;
                }
                d->num_bit =0;
            }
            else{
                d->num_bit ++;
            }
        }
        if(current_cel->symbol == 0xff){
            printf("ERORR");
        }
     
        int16_t symbol =current_cel->symbol;

        if(symbol == 0){
            while(cpt != 64){

                block[cpt] = 0;
                cpt ++;
            }
        }
        int8_t magnitude = (symbol & 0xF);
        int8_t num_zeros = (symbol >> 4);
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
                value = ((value << 1) + bit);

                d->num_bit++;
                if (d->num_bit == 8) {
                    fread(&d->byte, 1, 1, file);
                    if(d->find_ff){
                        fread(&d->byte, 1, 1, file);
                        d->find_ff = 0;
                    }
                    if(d->byte == 0xFF){
                        d->find_ff = 1;
                    }
                    d->num_bit = 0;
                }
                current_magnitude--;
            }

            if ((value & (1 << (magnitude - 1))) == 0) {
                value +=(-(1 << magnitude) + 1);
            }
            
            block[cpt] = value;
            cpt ++;
        }
        else{
            if(cpt < 64){
            block[cpt] =0;
            cpt ++;
            }
        }
        
        //printf("%x(%x) ",symbol,d->byte);
      
        current_cel = current_dht->racine_huffman;
    }
      //printf(" \n\n\n");
    return block;
}