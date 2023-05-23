#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/extract_bloc.h"


int8_t read_bit(BYTE byte, int8_t num_bit){
    /*Fonction prenant en entrée un octet et un numéro de bit et renvoyant
    la valeur correspondante au numéro de bitssssssssssssssssssssssssssssssssssssssssssss*/
    return (byte >> ( 7 - num_bit )) & 1 ;
}
void decode_ac_dc(struct data *d, int16_t index, int8_t table_type, FILE* file,  int16_t * block){
    /*Fonction prenant en entrée un pointeur vers notre structure data, un index, un type de table
    un pointeur vers un fichier et un pointeur vers un bloc et permettant de décoder les coefficients AC et DC*/

    // Déterminer la table de huffman (1 pour DC et 0 pour AC)
    struct dht_ac_dc *current_dht = table_type ?&d->list_dc[index] : &d->list_ac[index];
    
    // Lecture d'un nouvel octet si d->num_bit vaut -1
    if(d->num_bit == -1) {
        fread(&d->byte, 1, 1, file);
        d->num_bit = 0;
    }
    //printf("byte = %x\n", d->byte);

    int8_t bit;

    // Initialisation d'une cellule de huffman pour la racine
    struct cellule_huffman *current_cel = current_dht->racine_huffman;

    int8_t cpt =table_type ? 0 : 1;
    int8_t val = table_type ? 1 : 64;

    // Parcours de tous les coefficients
    while(cpt < val){
        // Parcours de l'arbre
        while(current_cel->right != NULL || current_cel->left != NULL){
            bit = read_bit(d->byte,d->num_bit);
            
            current_cel = bit ? current_cel->right : current_cel->left;
            // Lecture d'un nouvel octet si d->num_bit arrive à 7
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
        // On récupère le symbole quand on arrive à un noeud final
        int16_t symbol =current_cel->symbol;

        // Si le symbole vaut 0, le coefficient est nul et les coefficients restants dans la table AC sont mis à 0
        if(symbol == 0){
            while(cpt != 64){

                block[cpt] = 0;
                cpt ++;
            }
        }
        // On récupère la magnitude
        int8_t magnitude = (symbol & 0xF);
        int8_t num_zeros = (symbol >> 4);
        while (num_zeros > 0) {

            block[cpt] = 0;
            num_zeros--;
            cpt ++;
        }
        int16_t value = 0;

        int8_t current_magnitude =  magnitude;
        // Lecture des bits correspondant pour former la valeur du coefficient
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
            // Ajustement de la valeur du coefficient si le bit de poids fort est 0 car cela veut dire que la valeur est négative
            if ((value & (1 << (magnitude - 1))) == 0) {
                value +=(-(1 << magnitude) + 1);
            }
            
            block[cpt] = value;
            cpt ++;
        }
        // Tous les bits de magnitude sont lus
        else{
            // Ajout d'un coefficient nul si cpt<64
            if(cpt < 64){
            block[cpt] =0;
            cpt ++;
            }
        }
        
        //printf("%x(%x) ",symbol,d->byte);
      
        current_cel = current_dht->racine_huffman;
    }
}
