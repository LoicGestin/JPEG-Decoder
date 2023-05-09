#ifndef _DECODE_ENTETE_H_
#define _DECODE_ENTETE_H_


typedef	unsigned char BYTE;

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


struct data* init_data();

struct data* decode_entete(char * path);

#endif /* _DECODE_ENTETE_H_ */