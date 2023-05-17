#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/huffman.h"
#include "../include/decode_entete.h"

void insert_code_huffman(struct cellule_huffman **c,int16_t value, int16_t length, int16_t symbol){
    /*Fonction prenant en entrée un double pointeur vers une cellule de huffman qui est la racine,
    une valeur, une longueur et un symbole*/

    // Si l'arbre est nul, on l'initialise
    if (*c == NULL) {
        *c = malloc(sizeof(struct cellule_huffman));
        (*c)->symbol = -1;
        (*c)->left = NULL;
        (*c)->right = NULL;
    }
    // Si la longueur est <= 1, on est à la fin du code et on affecte le symbole à la cellule courante
    if (length <= 1) {
        (*c)->symbol = symbol;
    }
    // Sinon, il reste des bits et on continue récursivement
    else {
        int8_t is_bit_set = (value >> (length - 2)) & 1;
        if (is_bit_set) {
            insert_code_huffman(&((*c)->right), value, length - 1,symbol );
        } else {
            insert_code_huffman(&((*c)->left), value, length - 1, symbol);
        }
    }
}

void display_huffman_tree(struct cellule_huffman* c, char* path) {
    /*Fonction prenant en entrée un pointeur vers la racine de l'arbre et un 
    pointeur vers un chemin permettant d'afficher un arbre de huffman*/

    // Si l'arbre est NULL, on affiche rien
    if (c == NULL) {
        return;
    }
    // Si la cellule courante est une feuille, et que la longueur du path > 0, on affiche le chemin et le symbole qui correspond
    if (c->right == NULL && c->left == NULL && strlen(path) > 0) {
        printf("   path: %s, symbol: %x\n", path,c->symbol);
    // Sinon, on crée les branches gauche (ajout d'un 0) et droite (ajout d'un 1)
    } else {
        char* left_path = malloc(strlen(path)+  1);
        strcpy(left_path, path);
        strcat(left_path, "0");

        char* right_path = malloc(strlen(path)+ 1);
        strcpy(right_path, path);
        strcat(right_path, "1");
        // On appelle récursivement la fonction sur les branches gauche et droite
        display_huffman_tree(c->left, left_path);
        display_huffman_tree(c->right, right_path);

       // free(left_path);
       // free(right_path);
    }
}

void decode_huffman(struct dht_ac_dc *current_dht, int8_t table_type){
/*Fonction prenant en entrée une table de huffman et son type et
permettant de décoder cette table*/

    //struct dht_ac_dc *current_dht = table_type ? &d->list_dc[index] :  &d->list_ac[index];
    //current_dht->huff_code = malloc(current_dht->nb_symbols * sizeof(int));
    //current_dht->huff_length = malloc(current_dht->nb_symbols * sizeof(int));

    // Allocation de mémoire pour la racine et initialisation des attributs
    current_dht->racine_huffman = malloc(sizeof(struct cellule_huffman));
    current_dht->racine_huffman->symbol = -1;
    current_dht->racine_huffman->right = NULL;
    current_dht->racine_huffman->left = NULL;

    int16_t ind = 0;
    int16_t value = 0;
    int16_t length = 1;

    // Pour chacune des 16 longueurs
    for(int8_t i=0; i<16; i ++){
        // Pour chaque codes associées à cette longueur
        for(int16_t j=0; j < current_dht->nb_code[i]; j++){
            //current_dht->huff_code[ind] = value;
            //current_dht->huff_length[ind] = length;
            // Insertion du code de huffman dans l'arbre
            insert_code_huffman(&current_dht->racine_huffman,value,length+1,current_dht->huff_values[ind]);
            value ++;
            ind ++;
        }
        // Mise à jour de la valeur et de la longueur pour le prochain tour de boucle
        value <<=1;
        length ++;
    }


}

void free_huff(struct cellule_huffman *h){
    /*Fonction prenant en entrée un pointeur vers une cellule de huffman
    et permmettant de libérer la mémoire utilisée par un arbre de huffman */

    // Si h est Null, tout a été libéré. Sinon on libère les sous arbre gauche et droit et on finit par la racine
    if(h != NULL){
        free_huff(h->left);
        free_huff(h->right);
        free(h);
    }
}
