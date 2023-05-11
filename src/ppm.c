#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>


#include "../include/ppm.h"

void create_pgm_header(FILE *file_name, int16_t width, int16_t height) {
    if (file_name == NULL) {
        printf("Erreur dans la création du PGM.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(file_name, "P5\n");
    fprintf(file_name, "%d %d\n", width, height);
    fprintf(file_name, "255\n");

}

void create_pgm(FILE *file_name, uint8_t ***nuance, int16_t width, int16_t height) {
    int16_t nb_pixels_w = 0;
    int16_t nb_pixels_h = 0;

    int16_t taille_tab_h = width / 8 + ((width % 8 != 0) ? 1 : 0);
    if (width % 8 != 0 ||  height % 8 != 0 ){
        for (int8_t i = 0; i < 8; i++){
            for (int16_t k = 0; k < taille_tab_h; k++){
                for (int8_t j = 0; j < 8; j++){
                    if(nb_pixels_w >= width || nb_pixels_h >= height){
                        nb_pixels_w++;
                    }
                    else{
                        uint8_t pixel = nuance[k][i][j];
                        fwrite(&pixel, 1, 1, file_name);
                        nb_pixels_w++;
                    }
                }
            }
            nb_pixels_w = 0;
            nb_pixels_h++;
        }
    }
    else{
        for (int8_t i = 0; i < 8; i++) {
            for (int16_t k = 0; k < taille_tab_h; k++) {
                for (int8_t j = 0; j < 8; j++){
                    uint8_t pixel = nuance[k][i][j];
                    fwrite(&pixel, 1, 1, file_name);
                }
            }
        }
    }
}

void create_ppm_header(FILE *file_name, int16_t width, int16_t height) {
    if(file_name == NULL)
    {
        printf("Unable to create ppm file.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(file_name, "P6\n");
    fprintf(file_name, "%d %d\n", width, height);
    fprintf(file_name, "255\n");

}

void create_ppm(FILE * file_name, const uint8_t *** R, const uint8_t *** G, const uint8_t *** B, int16_t width, int16_t height){
    int16_t nb_pixels_w = 0;
    int16_t nb_pixels_h = 0;

    int16_t taille_tab_h = width / 8 + ((width % 8 != 0) ? 1 : 0);
    if (width % 8 != 0 ||  height % 8 != 0 ){
        for (int8_t i = 0; i < 8; i++){
            for (int16_t k = 0; k < taille_tab_h; k++){
                for (int8_t j = 0; j < 8; j++){
                    if(nb_pixels_w >= width || nb_pixels_h >= height){
                        nb_pixels_w++;
                    }
                    else{
                        uint8_t red_pixel = R[k][i][j];
                        uint8_t green_pixel = G[k][i][j];
                        uint8_t blue_pixel = B[k][i][j];

                        fputc(red_pixel, file_name);
                        fputc(green_pixel, file_name);
                        fputc(blue_pixel, file_name);
                        nb_pixels_w++;
                    }
                }
            }
            nb_pixels_w = 0;
            nb_pixels_h++;
        }
    }
    else{
        for (int8_t i = 0; i < 8; i++) {
            for (int16_t k = 0; k < taille_tab_h; k++) {
                for (int8_t j = 0; j < 8; j++){
                    uint8_t red_pixel = R[k][i][j];
                    uint8_t green_pixel = G[k][i][j];
                    uint8_t blue_pixel = B[k][i][j];

                    fputc(red_pixel, file_name);
                    fputc(green_pixel, file_name);
                    fputc(blue_pixel, file_name);
                }
            }
        }
    }

    // test si >0 et <256 ... voir si nécéssaire

}
