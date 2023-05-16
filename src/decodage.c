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
    struct data *d = decode_entete("../images/biiiiiig.jpg");

  
    int16_t cpt =0;
    int16_t nb_block_ligne = d->image_width / 8 + ((d->image_width % 8 != 0) ? 1 : 0);
    int16_t nb_block_colonne = d->image_height / 8 + ((d->image_height % 8 != 0) ? 1 : 0);
    
    int16_t precDC = 0;
    int16_t precDC_Cb = 0;
    int16_t precDC_Cr = 0;
    FILE *test_invaders = fopen("biiiiiig.ppm", "wb");
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
        struct component *comp = d->list_component;
        int8_t sampling_w = comp->sampling_horizontal;
        int8_t sampling_h = comp->sampling_vertical;

        nb_block_ligne = d->image_width / (8 * sampling_w) + ((d->image_width % (8 * sampling_w) != 0) ? 1 : 0);


       // printf(" ??? %d %d \n",sampling_w,sampling_h);
       //  printf(" ??? %d %d \n",nb_block_colonne,nb_block_ligne);
        uint8_t ***red=malloc(nb_block_ligne*sampling_w*sizeof(uint8_t **));
        uint8_t ***green=malloc(nb_block_ligne*sampling_w*sizeof(uint8_t **));
        uint8_t ***blue=malloc( nb_block_ligne*sampling_w*sizeof(uint8_t **));
        int16_t **Y = malloc((sampling_h * sampling_w)*sizeof(int16_t *));
        
        for(int16_t k = 0; k < nb_block_colonne/sampling_h; k++){
            for(int16_t i = 0; i < nb_block_ligne; i++){
            
                for(int16_t i = 0; i < sampling_h * sampling_w; i++){
                    Y[i] = malloc(64 * sizeof(int16_t));
                }
                
                int16_t *block_Cb = malloc(sizeof (int16_t) * 64);
                int16_t *block_Cr = malloc(sizeof (int16_t) * 64);

                //struct dht_ac_dc *current_dht = &d->list_dc[k];
                
                //display_huffman_tree(current_dht->racine_huffman,"");
                //struct dht_ac_dc *current_dht2 = &d->list_ac[k];
                //printf("\n\n");
                //display_huffman_tree(current_dht2->racine_huffman,"");

                for(int16_t i = 0; i < sampling_h * sampling_w; i++){
                    decode_ac_dc(d,d->list_scan_components[0].associated_dc_huffman_table_index,1,d->file,Y[i]);
                    decode_ac_dc(d,d->list_scan_components[0].associated_ac_huffman_table_index,0,d->file,Y[i]);

                    Y[i][0] = Y[i][0] + precDC;
                    precDC = Y[i][0];
                }
              

                decode_ac_dc(d,d->list_scan_components[1].associated_dc_huffman_table_index,1,d->file,block_Cb);
                decode_ac_dc(d,d->list_scan_components[1].associated_ac_huffman_table_index,0,d->file,block_Cb);
                block_Cb[0] = block_Cb[0] + precDC_Cb;
                precDC_Cb = block_Cb[0];
                /*for(int8_t i = 0; i < 64; i++){
                    printf("%hx ", block_Cb[i]);
                }
                    printf("\n");*/
                  
                decode_ac_dc(d,d->list_scan_components[2].associated_dc_huffman_table_index,1,d->file,block_Cr);
                decode_ac_dc(d,d->list_scan_components[2].associated_ac_huffman_table_index,0,d->file,block_Cr);
                block_Cr[0] = block_Cr[0] + precDC_Cr;
                precDC_Cr = block_Cr[0];
                /*for(int8_t i = 0; i < 64; i++){
                    printf("%hx ", block_Cr[i]);
                }
                printf("\n");*/

                for(int16_t i = 0; i < sampling_h * sampling_w; i++){
                    Y[i] = quantification_inverse(d,d->list_component[0].quantization_table_index,Y[i]);
                }
               
                
                int16_t * data_Cb = quantification_inverse(d,d->list_component[1].quantization_table_index,block_Cb);
                /*for(int8_t i = 0; i < 64; i++){
                    printf("%hx ", data_Cb[i]);
                }
                    printf("\n\n");*/
                int16_t * data_Cr = quantification_inverse(d,d->list_component[2].quantization_table_index,block_Cr);
                /*for(int8_t i = 0; i < 64; i++){
                    printf("%hx ", data_Cr[i]);
                }
                    printf("\n\n");*/

                
                int16_t ***matrice_Y = malloc((sampling_h * sampling_w)*sizeof(int16_t **));
                for(int16_t i = 0; i < sampling_h*sampling_w; i++){
                    matrice_Y[i] = zig_zag(Y[i]);
                }
                
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

                uint8_t ***pixel_Y = malloc((sampling_h * sampling_w)*sizeof(uint8_t **));
                for(int16_t i = 0; i < sampling_h*sampling_w; i++){
                    pixel_Y[i] = iDCT(matrice_Y[i]);
                }
                
                
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

                uint8_t **new_Y = malloc(8*sampling_h * sizeof(uint8_t *));
                for(int8_t i = 0; i < 8 * sampling_h; i++){
                    new_Y[i] = malloc(8 * sampling_w * sizeof(uint8_t));
                }
                
                for(int x= 0 ; x < sampling_h; x++){
                    for(int i=0; i< 8 ;i++){
                        for(int j=0; j < 8*sampling_w; j++){
                            //printf("x = %d, i = %d, j = %d\n", x,i,j);
                            new_Y[(x * 8) + i ][j] = pixel_Y[(x * sampling_w) + j/8][i][j%8];
                        }
                    }
                }
                /*
                for(int8_t x = 0; x < 16; x++){
                    for(int8_t j = 0; j < 8; j++){
                        printf("%hx ", new_Y[x][j]);
                    }
                    printf("\n");
                }
                printf("\n\n");
                */
                if( sampling_h != 1 || sampling_w != 1){
                    uint8_t **new_Cb = sur_ech(pixel_Cb, d);
                    uint8_t **new_Cr = sur_ech(pixel_Cr, d);
                    red[i] = YCbCr_to_R(new_Y, new_Cb, new_Cr, d);
                    green[i] = YCbCr_to_G(new_Y, new_Cb, new_Cr, d);
                    blue[i] = YCbCr_to_B(new_Y, new_Cb, new_Cr, d);
                }
                else{
                    red[i] = YCbCr_to_R(new_Y, pixel_Cb, pixel_Cr, d);
                    green[i] = YCbCr_to_G(new_Y, pixel_Cb, pixel_Cr, d);
                    blue[i] = YCbCr_to_B(new_Y, pixel_Cb, pixel_Cr, d);
                }
               
                /*printf("Cb\n");
                for(int8_t x = 0; x < 8; x++){
                    for(int8_t j = 0; j < 16; j++){
                        printf("%hx ", new_Cb[x][j]);
                    }
                    printf("\n");
                }
                printf("\n\n");*/

                
                /*printf("Cr\n");
                for(int8_t x = 0; x < 8; x++){
                    for(int8_t j = 0; j < 16; j++){
                        printf("%hx ", new_Cr[x][j]);
                    }
                    printf("\n");
                }
                printf("\n\n");*/
               
            }    
            
           
            create_ppm(test_invaders, red, green , blue, d);

        }

    } 
    fclose(test_invaders);       


    

   



    return EXIT_SUCCESS;
}
