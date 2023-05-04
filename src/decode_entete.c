#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef	unsigned char BYTE;
struct data{
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
    int nb_dht;
    int nb_ac;
    struct dht_ac* list_dht;
    struct dht_ac* list_ac;
    // SOS
    int nb_component_scan;
    struct scan_component* list_scan_components;

};
struct dht_ac{
    int table_type;// 0 = DC, 1 ==AC
    int table_index;
    int nb_symbols;
    int symbols[16];
};
struct component{
    int id;
    int sampling_horizontal;
    int sampling_vertical;
    int quantization_table_index;
};
struct scan_component {
    int scan_component_index;
    int associated_component_id;
    int associated_dc_huffman_table_index;
    int associated_ac_huffman_table_index;
};
struct data* init_data() {
    struct data* data = malloc(sizeof(struct data));

    // DQT
    data->quantization_precision =  malloc(4 * sizeof(int));
    data->quantization_table_read = malloc(4 * sizeof(int*));
    for (int i = 0; i < 4; i++) {
        data->quantization_table_read[i] = malloc(64 * sizeof(int));
    }
    // SOF0
    data->sample_precision = 0;
    data->image_height = 0;
    data->image_width = 0;
    data->nb_of_component = 4;
    data->list_component =  malloc(4 * sizeof(struct component));

    // DHT
    data->nb_ac = 4;
    data->nb_dht = 4;
    data->list_dht =  malloc(4 * sizeof(struct dht_ac ));
    data->list_ac =  malloc(4 * sizeof(struct dht_ac ));
    // SOS
    data->nb_component_scan = 4;
    data->list_scan_components =   malloc(4 * sizeof(struct scan_component));

    return data;
}

struct data* decode_entete(char * path){
    FILE* file = fopen("../images/invader.jpeg", "rb");

    if(file == NULL){
        printf("Erreur durant l'ouverture du fichier");
    }
    struct data *d = init_data();

    int cpt=0;
    BYTE byte;
    int marker_detected = 0;
    int marker_length = 0;
    while (fread(&byte, 1, 1, file)) {
        if(byte == 0xFF){
            marker_detected = 1;
            continue;
        }

        if(marker_detected) {
            if(byte == 0xD8){
                printf("[SOI] marker found\n");
                continue;
            }
            if(byte == 0xD9){
                printf("[EOI] marker found\n");
                break;
            }

            BYTE msb, lsb;

            fread(&msb, 1, 1, file);
            fread(&lsb, 1, 1, file);

            marker_length =(msb << 8) | lsb;

            BYTE *data =  malloc(sizeof(BYTE*) * marker_length -2 );
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
                    BYTE precision = data[0] == 0x00 ? 0x08 : 0x10 ;
                    printf("   quantization table index %d\n", index);
                    printf("   quantization table precision %d bits\n",precision);
                    printf("   quantization table read (64 bytes)\n");

                    d->quantization_precision[index] = precision;
                    for (int i = 0; i < 64; i++) {
                        d->quantization_table_read[index][i] = data[i + 1];
                    }

                    break;
                }
                case (0xC0):{
                    printf("[S0FO] length %d bytes\n", marker_length);
                    BYTE precision = data[0];
                    BYTE height = (data[1] << 8) | data[2];
                    BYTE width  = (data[3] << 8) | data[4];
                    printf("   sample precision %d\n",precision);
                    printf("   image height %d\n",height);
                    printf("   image width %d\n",width);
                    d->sample_precision = precision;
                    d->image_height = height;
                    d->image_width = width;
                    BYTE nb_components = data[5];
                    d->nb_of_component = nb_components;
                    printf("   nb of component %d\n",nb_components);

                    for (int i = 0; i < nb_components; i++) {
                        int offset = 6 + i * 3;
                        BYTE component_id = data[offset];
                        BYTE sampling_factors = data[offset + 1];
                        BYTE quantization_table_index = data[offset + 2];
                        printf("   component %d:\n", i + 1);
                        printf("     id: %d\n", component_id);
                        printf("     sampling factors (hxv) %dx%d\n", sampling_factors >> 4, sampling_factors & 0xF);
                        printf("     quantization table index: %d\n", quantization_table_index);
                        d->list_component[i].id = component_id;
                        d->list_component[i].sampling_horizontal = sampling_factors >> 4;
                        d->list_component[i].sampling_vertical = sampling_factors & 0xF;
                        d->list_component[i].quantization_table_index = quantization_table_index;
                    }
                    break;

                }
                case (0xC4): {
                    printf("[DHT] length %d bytes\n", marker_length);
                    char *type = ((data[0] >> 4) & 0x01) == 0x00? "DC" : "AC";
                    BYTE index = data[0] & 0x0F;
                    printf("   Huffman table type: %s\n", type );
                    printf("   Huffman table index : %d\n", index);

                    int total_symbols = 0;
                    BYTE symbols[16];
                    for (int i = 0;i < 16; i++) {
                        symbols[i] = data[i+1];
                        total_symbols += data[i+1];
                    }
                    if(total_symbols > 256){
                        printf("ERREUR NB TOTAL SYMBOLE > 256");
                    }
                    printf("   total nb of Huffman symbols %d\n", total_symbols);

                    break;
                }
                case(0xDA):{
                    printf("[SOS] length %d bytes\n", marker_length);
                    BYTE nb_composante = data[0];
                    printf("   nb of components in scan %d\n",nb_composante);
                    for (int i = 0;i < nb_composante; i++){
                        printf("   scan component index %d\n",i);
                        BYTE ic_composante = data[1 + (2*i)];
                        printf("     associated to component of id %d (frame index %d)\n",ic_composante,i);
                        BYTE id_DC_AC = data[2 + (2*i)];
                        printf("     associated to DC Huffman table of index %d\n",id_DC_AC >> 4);
                        printf("     associated to AC Huffman table of index %d\n",id_DC_AC & 0xF0);
                    }
                    printf("   other parameters ignored (%d bytes).\n", marker_length - 2 - 1 - nb_composante * 2);
                    printf("   End of Scan Header (SOS)\n");
                    break;
                }
                default:{
                    printf("[%x] Marker a implementer \n", byte);
                    break;
                }
            }
            free(data);


            marker_detected = 0;
        }


        /*
        if(cpt % 16 == 0){
            printf("%02x\n", byte );
        }
        else if(cpt % 8 == 0){
            printf("%02x  ", byte );
        }
        else{
            printf("%02x ", byte );
        }
        cpt += 1;
        */
    }
    printf("bitstream empty\n");
    fclose(file);
    return d;
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
    struct data *d = decode_entete("C:/Users/loicg/CLionProjects/team19/images/invader.jpeg");
    /* On se congratule. */
    return EXIT_SUCCESS;
}