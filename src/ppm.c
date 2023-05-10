#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

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

void create_pgm(FILE *file_name, uint8_t ***nuance, int16_t width) {
    int16_t taille_tab = width/8;
    for (int8_t i = 0; i < 8; i++) {
        for (int16_t k = 0; k < taille_tab; k++) {
            for (int8_t j = 0; j < 8; j++){
                uint8_t pixel = nuance[k][i][j];
                fwrite(&pixel, 1, 1, file_name);
            }
        }
    }
}

void create_ppm(char * file_name, const uint8_t ** R, const uint8_t ** G, const uint8_t ** B, int8_t width, int8_t height){
    FILE *ppm = fopen(file_name, "w");
    if(ppm == NULL)
    {
        printf("Unable to create ppm file.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(ppm, "P6\n");
    fprintf(ppm, "%d %d\n", width, height);
    fprintf(ppm, "255\n");
   for (int8_t i = 0; i < height; i++) {
        for (int8_t j = 0; j < width; j++) {

        uint8_t red_pixel = R[i][j];
        uint8_t green_pixel = G[i][j];
        uint8_t blue_pixel = B[i][j];

        fputc(red_pixel, ppm);
        fputc(green_pixel, ppm);
        fputc(blue_pixel, ppm);

        // test si >0 et <256 ... voir si nécéssaire
        }
    }
    fclose(ppm);
}
