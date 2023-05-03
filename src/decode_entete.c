#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct data{

};
typedef	unsigned char BYTE;
void decode_entete(){
    FILE* file = fopen("C:/Users/loicg/CLionProjects/team19/images/invader.jpeg", "rb");

    if(file == NULL){
        printf("Erreur durant l'ouverture du fichier");
    }
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

                    BYTE nb_components = data[5];
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
    decode_entete();
    /* On se congratule. */
    return EXIT_SUCCESS;
}