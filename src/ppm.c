#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/ppm.h"

void create_pgm(char *file_name, uint8_t **nuance, int width, int height) {
    FILE *pgm = fopen(file_name, "wb");
    if (pgm == NULL) {
        printf("Unable to create pgm file.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(pgm, "P5\n");
    fprintf(pgm, "%d %d\n", width, height);
    fprintf(pgm, "255\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8_t pixel = nuance[i][j];
            fwrite(&pixel, 1, 1, pgm);
        }
    }
    fclose(pgm);
}

void create_ppm(char * file_name, const int * R, const int * G, const int * B, int width, int height){
    FILE *ppm = fopen(file_name, "w");
    if(ppm == NULL)
    {
        printf("Unable to create ppm file.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(ppm, "P6\n");
    fprintf(ppm, "%d %d\n", width, height);
    fprintf(ppm, "255\n");
    for (int i = 0; i < height * width; i++) {

        int red_pixel = R[i];
        int green_pixel = G[i];
        int blue_pixel = B[i];

        fputc(red_pixel, ppm);
        fputc(green_pixel, ppm);
        fputc(blue_pixel, ppm);

        // test si >0 et <256 ... voir si nécéssaire

    }
    fclose(ppm);
}
