#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "../include/ppm.h"

void create_pgm(char *file_name, uint8_t **nuance, int8_t width, int8_t height) {
    FILE *pgm = fopen(file_name, "wb");
    if (pgm == NULL) {
        printf("Unable to create pgm file.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(pgm, "P5\n");
    fprintf(pgm, "%d %d\n", width, height);
    fprintf(pgm, "255\n");
    for (int8_t i = 0; i < height; i++) {
        for (int8_t j = 0; j < width; j++) {
            uint8_t pixel = nuance[i][j];
            fwrite(&pixel, 1, 1, pgm);
        }
    }
    fclose(pgm);
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
