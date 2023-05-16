#ifndef _DECODE_ENTETE_H_
#define _DECODE_ENTETE_H_


typedef	unsigned char BYTE;

struct data{
    FILE *file;
    BYTE byte;
    int8_t num_bit;
    int8_t find_ff;
    // DQT
    int8_t * quantization_precision;
    int8_t * quantization_index;
    int16_t ** quantization_table_read;
    // SOF0
    int16_t sample_precision;
    int16_t image_height;
    int16_t image_width;
    int8_t nb_of_component;
    struct component* list_component;
    // DHT
    struct dht_ac_dc* list_dc;
    struct dht_ac_dc* list_ac;
    // SOS
    int16_t nb_component_scan;
    struct scan_component* list_scan_components;
    int16_t **decoded_blocks;

};

struct dht_ac_dc{
    int8_t table_type;// 0 = AC, 1 ==DC
    //int table_index;
    int16_t nb_symbols;
    int8_t nb_code[16];
    int16_t *huff_values;
    struct cellule_huffman *racine_huffman;
};

struct cellule_huffman {
    int16_t symbol;
    struct cellule_huffman  *left;
    struct cellule_huffman  *right;
};

struct component{
    int8_t sampling_horizontal;
    int8_t sampling_vertical;
    int8_t quantization_table_index;
};

struct scan_component {
    int16_t scan_component_index;
    int8_t associated_dc_huffman_table_index;
    int8_t associated_ac_huffman_table_index;
};


struct data* init_data();

struct data* decode_entete(char * path);

#endif /* _DECODE_ENTETE_H_ */