#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "../include/decode_entete.h"
#include "../include/huffman.h"



struct data* init_data() {
    /*structure permettant de récupérer 
    les informations concernant les tables
    de quantification, les informations relatives
    à l'image, les informations sur les tables
    de huffman et les données brutes encodant l'image*/
    struct data* data = malloc(sizeof(struct data));
    data->find_ff = 0; 
    // DQT
    data->quantization_table_read = malloc(4 * sizeof(int16_t*));
    for (int i = 0; i < 4; i++) {
        data->quantization_table_read[i] = malloc(64 * sizeof(int16_t));
    }
    // SOF0
    data->nb_of_component = 4;
    data->list_component =  malloc(4 * sizeof(struct component));

    // DHT
    data->list_dc =  malloc(4 * sizeof(struct dht_ac_dc ));
    data->list_ac =  malloc(4 * sizeof(struct dht_ac_dc ));
    // SOS
    data->list_scan_components =  NULL;
    return data;
}

struct data* decode_entete(char * path){
    /*structure prenant en entrée un pointeur
    vers un chemin et permettant de décoder l'en-tête
    en lisant octet par octet et en détectant la
    présence des différents marqueurs pour récupérer
    les informations nécessaires au décodage de l'en -tête*/

    FILE* file = fopen(path, "rb");

    // Si le fichier n'existe pas on affiche un message d'erreur
    if(file == NULL){
        printf("Erreur durant l'ouverture du fichier");
    }
    // Initialisation de notre structure
    struct data *d = init_data();
    int8_t stop=0;
    BYTE byte;
    int8_t marker_detected = 0;
    int32_t marker_length = 0;
    // Tant que nous pouvons lire dans le flux
    while (fread(&byte, 1, 1, file)) {
        // Détection de marqueur avec "ff"
        if (byte == 0xFF) {
            marker_detected = 1;
            continue;
        }

        // Si on lit "ff" dans le flux on lit l'octet suivant pour déterminer le marqueur
        if (marker_detected) {
            // "d8" = marqueur de début d'image
            if (byte == 0xD8) {
                printf("[SOI] marker found\n\n");
                continue;
            }
            // "d9" = marqueur de fin d'image
            if (byte == 0xD9) {
                printf("[EOI] marker found\n\n");
                break;
            }

            BYTE msb, lsb;

            fread(&msb, 1, 1, file);
            fread(&lsb, 1, 1, file);
            marker_length = (msb << 8) | lsb;
            BYTE *data = malloc(sizeof(BYTE *) * marker_length - 2);
            // Pas oublier le -2 car les octets pour donner la taille font partie de la taille
            fread(data, marker_length - 2, 1, file);

            // Détection des autres marqueurs et récupération des informations
            switch (byte) {
                // image de type JFIF
                case (0xE0): {
                    printf("[APP0] length %d bytes\n", marker_length);
                    printf("  JFIF application\n");
                    printf("  other parameters ignored (%d bytes).\n", marker_length - 7);
                    break;
                }
                // table de quantification
                case (0xDB): {
                    printf("[DQT] length %d bytes\n", marker_length);
                    BYTE index = data[0] & 0x0F;
                    BYTE precision = (data[0] >> 4) == 0x00 ? 0x08 : 0x10;
                    printf("   quantization table index %d\n", index);
                    printf("   quantization table precision %d bits\n", precision);
                    printf("   quantization table read (64 bytes)\n\n");
                    for (int i = 0; i < 64; i++) {
                        d->quantization_table_read[index][i] = data[i + 1];
                    }
                    break;
                }
                // informations relatives à l'image
                case (0xC0): {
                    printf("[S0FO] length %d bytes\n", marker_length);                   
                    BYTE precision = data[0];
                    int16_t height = (data[1] << 8) | data[2];
                    int16_t width = (data[3] << 8) | data[4];
                    printf("   sample precision %d\n", precision);
                    printf("   image height %d\n", height);
                    printf("   image width %d\n", width);
                    d->sample_precision = precision;
                    d->image_height = height;
                    d->image_width = width;
                    BYTE nb_components = data[5];
                    d->nb_of_component = nb_components;
                    printf("   nb of component %d\n", nb_components);
                    // Pour chaque composant (Y,Cb,Cr), on détermine la composante iC, les facteurs d'échantillonnages et l'indice de table de quantification
                    for (int8_t i = 0; i < nb_components; i++) {
                        int32_t offset = 6 + i * 3;
                        BYTE component_id = data[offset];
                        BYTE sampling_factors = data[offset + 1];
                        BYTE quantization_table_index = data[offset + 2];
                        printf("   component %d:\n", i + 1);
                        printf("     id: %d\n", component_id);
                        printf("     sampling factors (hxv) %dx%d\n", sampling_factors >> 4, sampling_factors & 0xF);
                        printf("     quantization table index: %d\n\n", quantization_table_index);
                        d->list_component[i].sampling_horizontal = sampling_factors >> 4;
                        d->list_component[i].sampling_vertical = sampling_factors & 0xF;
                        d->list_component[i].quantization_table_index = data[offset + 2];
                    }
                    break;
                }
                // Table de huffman
                case (0xC4): {
                    printf("[DHT] length %d bytes\n", marker_length);
                    int8_t type = ((data[0] >> 4) & 0x01) == 0x00;
                    int16_t index = data[0] & 0x0F;
                    printf("   Huffman table type: %s\n", type ? "DC" : "AC");
                    printf("   Huffman table index : %d\n", index);
                    // Décompte du nombre de symboles (16 octets donnant le nombre de codes de longueur 1 à 16)
                    int16_t total_symbols = 0;
                    BYTE nb_code[16];
                    for (int8_t i = 0; i < 16; i++) {
                        nb_code[i] = data[i + 1];
                        total_symbols += data[i + 1];
                    }
                    // On affiche un message d'erreur si le nombre de symboles est supérieur à 256
                    if (total_symbols > 256) {
                        printf("ERREUR NB TOTAL SYMBOLE > 256");
                    }
                    printf("   total nb of Huffman symbols %d\n", total_symbols);
                    // Initialisation de la table de huffman courante
                    struct dht_ac_dc *current_dht = NULL;
                    if (type) {
                        current_dht = &d->list_dc[index];
                    } else {
                        current_dht = &d->list_ac[index];
                    }
                    current_dht->table_type = type;
                    current_dht->nb_symbols = total_symbols;
                    for (int8_t i = 0; i < 16; i++) {
                        current_dht->nb_code[i] = nb_code[i];
                    }
                    current_dht->huff_values = malloc(total_symbols * sizeof(int16_t));
                    for (int16_t i = 0, offset = 17; i < total_symbols; i++, offset++) {
                        current_dht->huff_values[i] = data[offset];
                    }
                    decode_huffman(current_dht);
                    printf("\n");
                    break;
                }
                // Données encodées
                case (0xDA): {
                    printf("[SOS] length %d bytes\n", marker_length);
                    BYTE nb_composante = data[0];
                    d->nb_component_scan = nb_composante;
                    printf("   nb of components in scan %d\n", nb_composante);
                    d->list_scan_components = malloc(nb_composante * sizeof(struct scan_component));
                    // Pour chaque composante, on associe un identifiant, une table de huffman AC et DC
                    for (int16_t i = 0; i < nb_composante; i++) {
                        printf("   scan component index %d\n", i);
                        BYTE ic_composante = data[1 + (2 * i)];
                        printf("     associated to component of id %d (frame index %d)\n", ic_composante, i);
                        BYTE id_DC_AC = data[2 + (2 * i)];
                        printf("     associated to DC Huffman table of index %d\n", id_DC_AC >> 4);
                        printf("     associated to AC Huffman table of index %d\n", id_DC_AC & 0xF);
                        d->list_scan_components[i].scan_component_index = ic_composante;
                        d->list_scan_components[i].associated_dc_huffman_table_index = id_DC_AC >> 4;
                        d->list_scan_components[i].associated_ac_huffman_table_index = id_DC_AC & 0xF;
                    }
                    printf("   other parameters ignored (%d bytes).\n", marker_length - 2 - 1 - nb_composante * 2);
                    printf("   End of Scan Header (SOS)\n\n");
                    stop = 1;
                    break;
                }
                // Commentaires
                case(0xFE):{
                    printf("[COM] length %d bytes\n", marker_length);
                    printf("   ");
                    for(int i = 0; i < marker_length -2; i ++){
                        printf("%c",data[i]);
                    }
                    printf("\n");
                    break;
                }
                // Si on détecte un autre marqueur, il faudra l'implémenter
                default: {
                    printf("[%x] Marker a implementer \n", byte);
                    break;
                }
            }
            free(data);
            marker_detected = 0;
        }
        if(stop){
            break;
        }
    }

    d->file = file;
    d->byte = byte;
    d->num_bit = -1;
    return d;
}


