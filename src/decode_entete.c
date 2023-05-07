#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "../include/decode_entete.h"
struct data{
    FILE *file;
    BYTE byte;
    int num_bit;
    // DQT
    int * quantization_precision;
    int ** quantization_table_read;
    // SOF0
    int sample_precision;
    int image_height;
    int image_width;
    int nb_of_component;
    struct component* list_component;
    // DHT
    int nb_ac;
    int nb_dc;
    struct dht_ac_dc* list_dc;
    struct dht_ac_dc* list_ac;
    // SOS
    int nb_component_scan;
    struct scan_component* list_scan_components;
    int **decoded_blocks;

};
struct dht_ac_dc{
    int table_type;// 0 = AC, 1 ==DC
    int table_index;
    int nb_symbols;
    int nb_code[16];
    BYTE *huff_values;
    int *huff_code;
    int *huff_length;
    struct cellule_huffman *racine_huffman;
};
struct cellule_huffman {
    BYTE symbol;
    struct cellule_huffman  *left;
    struct cellule_huffman  *right;
};
struct component{
    int id;
    int sampling_horizontal;
    int sampling_vertical;
    int quantization_table_index;
};
struct scan_component {
    int scan_component_index;
    int associated_dc_huffman_table_index;
    int associated_ac_huffman_table_index;
};

void insert_code_huffman(struct cellule_huffman **c,int value, int length, BYTE symbol){
    if (*c == NULL) {
        *c = malloc(sizeof(struct cellule_huffman));
        (*c)->symbol = -1;
        (*c)->left = NULL;
        (*c)->right = NULL;
    }
    if (length <= 1) {
        (*c)->symbol = symbol;
    }
    else {
        int is_bit_set = (value >> (length - 2)) & 1;
        if (is_bit_set) {
            insert_code_huffman(&((*c)->right), value, length - 1,symbol );
        } else {
            insert_code_huffman(&((*c)->left), value, length - 1, symbol);
        }
    }
}
void display_huffman_tree(struct cellule_huffman* c, char* path) {
    if (c == NULL) {
        return;
    }

    if (c->right == NULL && c->left == NULL && strlen(path) > 0) {
        printf("   path: %s, symbol: %x\n", path,c->symbol);
    } else {
        char* left_path = malloc(strlen(path)+  1);
        strcpy(left_path, path);
        strcat(left_path, "0");

        char* right_path = malloc(strlen(path)+ 1);
        strcpy(right_path, path);
        strcat(right_path, "1");

        display_huffman_tree(c->left, left_path);
        display_huffman_tree(c->right, right_path);

       // free(left_path);
       // free(right_path);
    }
}
void decode_huffman(struct data* d, int index, int table_type){

    struct dht_ac_dc *current_dht = table_type ? &d->list_dc[index] :  &d->list_ac[index];
    //current_dht->huff_code = malloc(current_dht->nb_symbols * sizeof(int));
    //current_dht->huff_length = malloc(current_dht->nb_symbols * sizeof(int));
    current_dht->racine_huffman = malloc(sizeof(struct cellule_huffman));
    current_dht->racine_huffman->symbol = -1;
    current_dht->racine_huffman->right = NULL;
    current_dht->racine_huffman->left = NULL;

    int ind = 0;
    int value = 0;
    int length = 1;
    for(int i=0; i<16; i ++){

        for(int j=0; j < current_dht->nb_code[i]; j++){
            //current_dht->huff_code[ind] = value;
            //current_dht->huff_length[ind] = length;
            insert_code_huffman(&current_dht->racine_huffman,value,length+1,current_dht->huff_values[ind]);
            value ++;
            ind ++;
        }
        value <<=1;
        length ++;
    }


}

/*
char * decimal_to_binary(int val, int length){
    char *res = malloc(length + 1);
    res[length] = '\0';

    for (int i = length - 1; i >= 0; i--) {

        res[i] = (val & 1) + '0';
        val >>= 1;
    }

    return res;
}
*/


struct data* init_data() {
    struct data* data = malloc(sizeof(struct data));

    // DQT
    data->quantization_precision =  malloc(4 * sizeof(int));
    data->quantization_table_read = malloc(4 * sizeof(int*));
    for (int i = 0; i < 4; i++) {
        data->quantization_table_read[i] = malloc(64 * sizeof(int));
    }
    // SOF0

    data->nb_of_component = 4;
    data->list_component =  malloc(4 * sizeof(struct component));

    // DHT
    data->nb_ac = 4;
    data->nb_dc = 4;
    data->list_dc =  malloc(4 * sizeof(struct dht_ac_dc ));
    data->list_ac =  malloc(4 * sizeof(struct dht_ac_dc ));
    // SOS
    data->nb_component_scan = 0;
    data->list_scan_components =  NULL;

    return data;
}
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

uint16_t * decode_ac_dc(struct data *d, int index, int table_type,FILE* file,  uint16_t * block){
    struct dht_ac_dc *current_dht = table_type ?&d->list_dc[index] : &d->list_ac[index];


    if(d->num_bit == -1) {
        fread(&d->byte, 1, 1, file);
        d->num_bit = 0;
    }
    printf("byte = %x\n", d->byte);

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
        BYTE symbol = current_cel->symbol;
        if(symbol == 0){
            while(cpt != 64){

                block[cpt] = 0;
                cpt ++;
            }
        }
        int magnitude = symbol & 0xF;
        int num_zeros = symbol >> 4;
        while (num_zeros > 0) {

            block[cpt] = 0;
            num_zeros--;
            cpt ++;
        }
        int value = 0;

        int current_magnitude = magnitude;
        if(current_magnitude > 0) {
            while (current_magnitude > 0) {
                bit = read_bit(d->byte, d->num_bit);
                value = (value << 1) + bit;

                d->num_bit++;
                if (d->num_bit == 8) {
                    fread(&d->byte, 1, 1, file);
                    d->num_bit = 0;
                }
                current_magnitude--;
            }

            if ((value & (1 << (magnitude - 1))) == 0) {
                value += -(1 << magnitude) + 1;
            }

            block[cpt] = value;
            cpt ++;
        }
        else{

            block[cpt] =0;
            cpt ++;
        }
        printf("byte %x, symbole %x, magnitude = %x, value = %x , num_zero %d, cpt = %d\n",symbol,magnitude,value,symbol >> 4,cpt );
        current_cel = current_dht->racine_huffman;
    }
    for(int i =0; i < 64; i++){
        printf("%x ", block[i]);
    }
    return block;
}

struct data* decode_entete(char * path){
    FILE* file = fopen(path, "rb");

    if(file == NULL){
        printf("Erreur durant l'ouverture du fichier");
    }
    struct data *d = init_data();
    int stop=0;
    BYTE byte;
    int marker_detected = 0;
    int marker_length = 0;
    while (fread(&byte, 1, 1, file)) {
        if (byte == 0xFF) {
            marker_detected = 1;
            continue;
        }

        if (marker_detected) {
            if (byte == 0xD8) {
                printf("[SOI] marker found\n\n");
                continue;
            }
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

            switch (byte) {
                case (0xE0): {
                    printf("[APP0] length %d bytes\n", marker_length);
                    printf("  JFIF application\n");
                    printf("  other parameters ignored (%d bytes).\n", marker_length - 7);
                    break;
                }
                case (0xDB): {
                    printf("[DQT] length %d bytes\n", marker_length);
                    BYTE index = data[0] & 0x0F;
                    BYTE precision = data[0] == 0x00 ? 0x08 : 0x10;
                    printf("   quantization table index %d\n", index);
                    printf("   quantization table precision %d bits\n", precision);
                    printf("   quantization table read (64 bytes)\n\n");

                    d->quantization_precision[index] = precision;
                    for (int i = 0; i < 64; i++) {
                        d->quantization_table_read[index][i] = data[i + 1];
                    }

                    break;
                }
                case (0xC0): {
                    printf("[S0FO] length %d bytes\n", marker_length);
                    BYTE precision = data[0];
                    BYTE height = (data[1] << 8) | data[2];
                    BYTE width = (data[3] << 8) | data[4];
                    printf("   sample precision %d\n", precision);
                    printf("   image height %d\n", height);
                    printf("   image width %d\n", width);
                    d->sample_precision = precision;
                    d->image_height = height;
                    d->image_width = width;
                    BYTE nb_components = data[5];
                    d->nb_of_component = nb_components;
                    printf("   nb of component %d\n", nb_components);

                    for (int i = 0; i < nb_components; i++) {
                        int offset = 6 + i * 3;
                        BYTE component_id = data[offset];
                        BYTE sampling_factors = data[offset + 1];
                        BYTE quantization_table_index = data[offset + 2];
                        printf("   component %d:\n", i + 1);
                        printf("     id: %d\n", component_id);
                        printf("     sampling factors (hxv) %dx%d\n", sampling_factors >> 4, sampling_factors & 0xF);
                        printf("     quantization table index: %d\n\n", quantization_table_index);
                        d->list_component[i].id = component_id;
                        d->list_component[i].sampling_horizontal = sampling_factors >> 4;
                        d->list_component[i].sampling_vertical = sampling_factors & 0xF;
                        d->list_component[i].quantization_table_index = quantization_table_index;
                    }
                    break;

                }
                case (0xC4): {
                    printf("[DHT] length %d bytes\n", marker_length);
                    int type = ((data[0] >> 4) & 0x01) == 0x00;
                    BYTE index = data[0] & 0x0F;
                    printf("   Huffman table type: %s\n", type ? "DC" : "AC");
                    printf("   Huffman table index : %d\n", index);

                    int total_symbols = 0;
                    BYTE nb_code[16];
                    for (int i = 0; i < 16; i++) {
                        nb_code[i] = data[i + 1];
                        total_symbols += data[i + 1];
                    }
                    if (total_symbols > 256) {
                        printf("ERREUR NB TOTAL SYMBOLE > 256");
                    }
                    printf("   total nb of Huffman symbols %d\n", total_symbols);
                    struct dht_ac_dc *current_dht = NULL;
                    if (type) {
                        d->nb_dc++;
                        current_dht = &d->list_dc[index];
                    } else {
                        d->nb_ac++;
                        current_dht = &d->list_ac[index];
                    }
                    current_dht->table_type = type;


                    current_dht->table_index = index;
                    current_dht->nb_symbols = total_symbols;
                    for (int i = 0; i < 16; i++) {
                        current_dht->nb_code[i] = nb_code[i];

                    }
                    current_dht->huff_values = malloc(total_symbols * sizeof(BYTE));
                    for (int i = 0, offset = 17; i < total_symbols; i++, offset++) {
                        current_dht->huff_values[i] = data[offset];
                    }
                    decode_huffman(d, index, type);
                    /*
                    for (int i = 0; i < total_symbols; i++) {
                        printf("   path: %s symbol: %x\n",
                               decimal_to_binary(current_dht->huff_code[i], current_dht->huff_length[i]),
                               current_dht->huff_values[i]);
                    }
                     */
                    char initial_path[1] = "";
                    display_huffman_tree(current_dht->racine_huffman, initial_path);
                    printf("\n");
                    init_blocks(d);

                    break;
                }
                case (0xDA): {
                    printf("[SOS] length %d bytes\n", marker_length);
                    BYTE nb_composante = data[0];
                    d->nb_component_scan = nb_composante;
                    printf("   nb of components in scan %d\n", nb_composante);
                    d->list_scan_components = malloc(nb_composante * sizeof(struct scan_component));
                    for (int i = 0; i < nb_composante; i++) {
                        printf("   scan component index %d\n", i);
                        BYTE ic_composante = data[1 + (2 * i)];
                        printf("     associated to component of id %d (frame index %d)\n", ic_composante, i);
                        BYTE id_DC_AC = data[2 + (2 * i)];
                        printf("     associated to DC Huffman table of index %d\n", id_DC_AC >> 4);
                        printf("     associated to AC Huffman table of index %d\n", id_DC_AC & 0xF0);
                        d->list_scan_components[i].scan_component_index = ic_composante;
                        d->list_scan_components[i].associated_dc_huffman_table_index = id_DC_AC >> 4;
                        d->list_scan_components[i].associated_ac_huffman_table_index = id_DC_AC & 0xF;
                    }
                    printf("   other parameters ignored (%d bytes).\n", marker_length - 2 - 1 - nb_composante * 2);
                    printf("   End of Scan Header (SOS)\n\n");
                    stop = 1;
                    break;
                }
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
int * quantification_inverse(struct data *d, int index,  const uint16_t *frequentiel){
    int *quantification = d->quantization_table_read[index];
    int *table = malloc(64*sizeof(int*));
    for(int i=0; i<64; i++){
        table[i]= frequentiel[i]*quantification[i];
    }

    return table;
}


int ** zig_zag(const int *tab){


    int **matrice = malloc(8*sizeof(int*));
    for(int i=0; i<8; i++){
        matrice[i]=malloc(8*sizeof(int));
    }
    int i =0;
    int j=0;
    int k=0;
    matrice[i][j]=tab[k];
    j++;
    k++;
    while (k<64){
        if (k<35){
            if(j%2 && i==0){
                while(j!=0){
                    matrice[i][j]=tab[k];
                    j--;
                    i++;
                    k++;
                }
                matrice[i][j]=tab[k];
                k++;
            }
            else if(i%2==0 && j==0){
                while(i!=0){
                    matrice[i][j]=tab[k];
                    i--;
                    j++;
                    k++;
                }
                matrice[i][j]=tab[k];
                k++;
            }
            else if(j%2==0 && i==0){
                j++;
            }
            else if(i%2 && j==0){
                i++;
            }}

        else {
            if(j%2 && i==7){
                while(j!=7){
                    matrice[i][j]=tab[k];
                    j++;
                    i--;
                    k++;
                }
                matrice[i][j]=tab[k];
                k++;
            }
            else if(i%2==0 && j==7){
                while(i!=7){
                    matrice[i][j]=tab[k];
                    i++;
                    j--;
                    k++;
                }
                matrice[i][j]=tab[k];
                k++;
            }
            else if(j%2==0 && i==7){
                j++;
            }
            else if(i%2 && j==7){
                i++;
            }
        }
    }


    return matrice;
}
uint8_t iDCT(int x, int y , int **phi){
    int n = 8;

    float dep = (1 / pow(2*n, 1/2));
    float t1 = (1/2) * phi[0][0];

    float t2 = 0.0;
    for (int mu = 1; mu<n; mu++){
        t2 += (1 / pow(2*n, 1/2)) * cosf(((2*x+1)*mu*M_PI)/(2*n)) * phi[0][mu];
    }

    float t3 = 0.0;
    for (int lambda = 1; lambda<n; lambda++){
        t3 += (1 / pow(2*n, 1/2)) * cosf(((2*y+1)*lambda*M_PI)/(2*n)) * phi[lambda][0];
    }

    float t4 = 0.0;
    for (int lambda = 1; lambda<n; lambda++){
        for (int mu = 1; mu<n; mu++){
            t4 += cosf(((2*x+1)*mu*M_PI)/(2*n)) * cosf(((2*y+1)*lambda*M_PI)/(2*n)) * phi[lambda][mu];
        }
    }

    float S = dep * (t1 + t2 + t3 + t4);
    if (S<0.0){
        S=0;
    }
    else if(S>255.0){
        S=255;
    }


    S = (uint8_t)(S);


    return S;
}
void create_pgm(char * file_name,  int **nuance, int width, int height){
    FILE *pgm = fopen(file_name, "w");
    if(pgm == NULL)
    {
        printf("Unable to create pgm file.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(pgm, "P5\n");
    fprintf(pgm, "%d %d\n", width, height);
    fprintf(pgm, "255\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel = nuance[i][j];
            if (pixel < 0) {
                pixel = 0;
            } else if (pixel > 255) {
                pixel = 255;
            }
            fputc(pixel, pgm);
        }
    }
    fclose(pgm);
}
int main(int argc, char **argv)
{
    struct data *d = decode_entete("C:/Users/loicg/CLionProjects/team19/images/invader.jpeg");

    printf("%x \n", d->byte);
    uint16_t *block = malloc(sizeof (uint16_t) * 64);
    //fread(&d->byte, 1, 1, d->file);
    decode_ac_dc(d,0,1,d->file,block);
    decode_ac_dc(d,0,0,d->file,block);
    printf("\n\n");
    int * data = quantification_inverse(d,0,block);

    for(int i =0; i < 64; i++){
        printf("%x ", data[i]);
    }
    printf("\n\n");
    int ** matrice = zig_zag(data);
    for(int i =0; i < 8; i++){
        for(int j =0; j < 8; j++){
            printf("%x ", matrice[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    for(int i =0; i < 8; i++){
        for(int j =0; j < 8; j++){
            matrice[i][j] = iDCT(i,j,matrice);
        }
    }
    printf("\n\n");
    for(int i =0; i < 8; i++){
        for(int j =0; j < 8; j++){
            printf("%x ", matrice[i][j]);
        }
        printf("\n");
    }
    create_pgm("test_invaders.pgm",matrice,8,8);
    //problème ordre, manque upsampling ?


    return EXIT_SUCCESS;
}