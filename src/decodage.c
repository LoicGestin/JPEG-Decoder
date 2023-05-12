#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "../include/decode_entete.h"
#include "../include/huffman.h"
#include "../include/extract_bloc.h"
#include "../include/iDCT.h"
#include "../include/couleur_matrice.h"
#include "../include/ppm.h"
#include "../include/upsampling.h"
#include "../include/quantification_inverse.h"
#include "../include/zig_zag.h"


int main(int argc, char **argv){
    struct data *d = decode_entete("../images/poupoupidou.jpg");

    printf("%x \n", d->byte);
    int16_t cpt =0;
    int16_t nb_block_ligne = d->image_width / 8 + ((d->image_width % 8 != 0) ? 1 : 0);
    int16_t nb_block_colonne = d->image_height / 8 + ((d->image_height % 8 != 0) ? 1 : 0);
    int16_t precDC = 0;
    int16_t precDC_Cb = 0;
    int16_t precDC_Cr = 0;
    FILE *test_invaders = fopen("poupoupidou.ppm", "wb");
    if(d->nb_component_scan == 1){
        uint8_t ***mat=malloc(nb_block_ligne*sizeof(uint8_t **));
        create_pgm_header(test_invaders, d->image_width, d->image_height);
        for(int16_t k = 0; k < nb_block_colonne; k++){
            for (int16_t i = 0; i < nb_block_ligne; i++){
                int16_t *block = malloc(sizeof (int16_t) * 64);
                //fread(&d->byte, 1, 1, d->file);
                decode_ac_dc(d,0,1,d->file,block);
                decode_ac_dc(d,0,0,d->file,block);
                block[0] = block[0] + precDC;
                precDC = block[0];
                printf("\n\n");

                int16_t * data = quantification_inverse(d,0,block);

                for(int8_t i = 0; i < 64; i++){
                    printf("%hx ", data[i]);
                }
                printf("\n\n");
                int16_t ** matrice = zig_zag(data);
                for(int8_t x = 0; x < 8; x++){
                    for(int8_t j = 0; j < 8; j++){
                        printf("%hx ", matrice[x][j]);
                    }
                    printf("\n");
                }
                printf("\n\n");
                
                
                printf("\n");
                uint8_t **pixel= iDCT(matrice);
                
                for(int8_t x = 0; x < 8; x++){
                    for(int8_t j = 0; j < 8; j++){
                        printf("%hx ", pixel[x][j]);
                    }
                    printf("\n");
                }
                
                cpt++;
                mat[i]=pixel;
                free(block);
                printf("%d\n", cpt);
                printf("%d\n", nb_block_ligne);
                printf("%d\n", nb_block_colonne);
            }
            create_pgm(test_invaders ,mat, d->image_width, d->image_height);
        }
        fclose(test_invaders);
}
else{
    
    create_ppm_header(test_invaders, d->image_width, d->image_height);
    uint8_t ***red=malloc(2*sizeof(uint8_t **));
    uint8_t ***green=malloc(2*sizeof(uint8_t **));
    uint8_t ***blue=malloc(2*sizeof(uint8_t **));
    for(int16_t k = 0; k < nb_block_colonne; k++){
        
            int16_t *block_Y = malloc(sizeof (int16_t) * 64);
            int16_t *block_Y2 = malloc(sizeof (int16_t) * 64);
            int16_t *block_Cb = malloc(sizeof (int16_t) * 64);
            int16_t *block_Cr = malloc(sizeof (int16_t) * 64);

            //struct dht_ac_dc *current_dht = &d->list_dc[k];
            
            //display_huffman_tree(current_dht->racine_huffman,"");
            //struct dht_ac_dc *current_dht2 = &d->list_ac[k];
            //printf("\n\n");
            //display_huffman_tree(current_dht2->racine_huffman,"");


            decode_ac_dc(d,0,1,d->file,block_Y);
            decode_ac_dc(d,0,0,d->file,block_Y);

            block_Y[0] = block_Y[0] + precDC;
            precDC = block_Y[0];
            /*for(int8_t i = 0; i < 64; i++){
                printf("%hx ", block_Y[i]);
            }
             printf("\n");*/

            decode_ac_dc(d,0,1,d->file,block_Y2);
            decode_ac_dc(d,0,0,d->file,block_Y2);
            block_Y2[0] = block_Y2[0] + precDC;
            precDC = block_Y2[0];
            /*for(int8_t i = 0; i < 64; i++){
                printf("%hx ", block_Y2[i]);
            }
             printf("\n");*/

            decode_ac_dc(d,1,1,d->file,block_Cb);
            decode_ac_dc(d,1,0,d->file,block_Cb);
            block_Cb[0] = block_Cb[0] + precDC_Cb;
            precDC_Cb = block_Cb[0];
            /*for(int8_t i = 0; i < 64; i++){
                printf("%hx ", block_Cb[i]);
            }
             printf("\n");*/

            decode_ac_dc(d,1,1,d->file,block_Cr);
            decode_ac_dc(d,1,0,d->file,block_Cr);
            block_Cr[0] = block_Cr[0] + precDC_Cr;
            precDC_Cr = block_Cr[0];
            /*for(int8_t i = 0; i < 64; i++){
                printf("%hx ", block_Cr[i]);
            }
            printf("\n");*/

            int16_t * data_Y = quantification_inverse(d,0,block_Y);
            /*for(int8_t i = 0; i < 64; i++){
                printf("%hx ", data_Y[i]);
            }
             printf("\n\n");*/
            int16_t * data_Y2 = quantification_inverse(d,0,block_Y2);
            /*for(int8_t i = 0; i < 64; i++){
                printf("%hx ", data_Y2[i]);
            }
             printf("\n\n");*/
            int16_t * data_Cb = quantification_inverse(d,1,block_Cb);
            /*for(int8_t i = 0; i < 64; i++){
                printf("%hx ", data_Cb[i]);
            }
             printf("\n\n");*/
            int16_t * data_Cr = quantification_inverse(d,1,block_Cr);
            /*for(int8_t i = 0; i < 64; i++){
                printf("%hx ", data_Cr[i]);
            }
             printf("\n\n");*/

            int16_t ** matrice_Y = zig_zag(data_Y);
            /*for(int8_t x = 0; x < 8; x++){
                for(int8_t j = 0; j < 8; j++){
                    printf("%hx ", matrice_Y[x][j]);
                }
                printf("\n");
            }
            printf("\n\n");*/
            int16_t ** matrice_Y2 = zig_zag(data_Y2);
            /*for(int8_t x = 0; x < 8; x++){
                for(int8_t j = 0; j < 8; j++){
                    printf("%hx ", matrice_Y2[x][j]);
                }
                printf("\n");
            }
            printf("\n\n");*/
            int16_t ** matrice_Cb = zig_zag(data_Cb);
            /*for(int8_t x = 0; x < 8; x++){
                for(int8_t j = 0; j < 8; j++){
                    printf("%hx ", matrice_Cb[x][j]);
                }
                printf("\n");
            }
            printf("\n\n");*/
            int16_t ** matrice_Cr = zig_zag(data_Cr);
            /*for(int8_t x = 0; x < 8; x++){
                for(int8_t j = 0; j < 8; j++){
                    printf("%hx ", matrice_Cr[x][j]);
                }
                printf("\n");
            }
            printf("\n\n");*/

            uint8_t **pixel_Y= iDCT(matrice_Y);
            /*for(int8_t x = 0; x < 8; x++){
                for(int8_t j = 0; j < 8; j++){
                    printf("%hx ", pixel_Y[x][j]);
                }
                printf("\n");
            } 
            printf("\n\n");*/
            uint8_t **pixel_Y2= iDCT(matrice_Y2);
            /*for(int8_t x = 0; x < 8; x++){
                for(int8_t j = 0; j < 8; j++){
                    printf("%hx ", pixel_Y2[x][j]);
                }
                printf("\n");
            }
            printf("\n\n");*/
            uint8_t **pixel_Cb= iDCT(matrice_Cb);
            /*for(int8_t x = 0; x < 8; x++){
                for(int8_t j = 0; j < 8; j++){
                    printf("%hx ", pixel_Cb[x][j]);
                }
                printf("\n");
            }
            printf("\n\n");*/
            uint8_t **pixel_Cr= iDCT(matrice_Cr);
            /*for(int8_t x = 0; x < 8; x++){
                for(int8_t j = 0; j < 8; j++){
                    printf("%hx ", pixel_Cr[x][j]);
                }
                printf("\n");
            }
            printf("\n\n");*/

            uint8_t **pixel_Cb1 = sur_ech_h_Cb1(pixel_Cb);
            uint8_t **pixel_Cb2 = sur_ech_h_Cb1(pixel_Cb);
            uint8_t **pixel_Cr1 = sur_ech_h_Cb1(pixel_Cr);
            uint8_t **pixel_Cr2 = sur_ech_h_Cb1(pixel_Cr);

            red[1] = YCbCr_to_R(pixel_Y, pixel_Cb1, pixel_Cr1);
            green[1] = YCbCr_to_G(pixel_Y, pixel_Cb1, pixel_Cr1);
            blue[1] = YCbCr_to_B(pixel_Y, pixel_Cb1, pixel_Cr1);
            red[2] = YCbCr_to_R(pixel_Y2, pixel_Cb2, pixel_Cr2);
            green[2] = YCbCr_to_G(pixel_Y2, pixel_Cb2, pixel_Cr2);
            blue[2] = YCbCr_to_B(pixel_Y2, pixel_Cb2, pixel_Cr2);

            

        
        create_ppm(test_invaders, red, green , blue, d->image_width, d->image_height);
    } 
    fclose(test_invaders);       

}
    

   



    return EXIT_SUCCESS;
}