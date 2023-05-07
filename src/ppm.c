#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void create_pgm(char * file_name,  int **nuance, int width, int height){
    FILE *pgm = fopen(file_name, "w");
    if(pgm == NULL)
    {
        printf("Unable to create pgm file.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(pgm, "P5\n");
    fprintf(pgm, "%d %d\n", width, height);
    fprintf(pgm, "255\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel = nuance[i][j];
            if (pixel < 0) {
                pixel = 0;
            } else if (pixel > 255) {
                pixel = 255;
            }
            fputc(pixel, pgm);
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
int main(int argc, char **argv)
{
    int * nuance = malloc(sizeof(int) * 10000);
    for(int i = 0; i < 10000; i++){
        nuance[i] = i * 52 % 255;
    }
    int * R = malloc(sizeof(int) * 10000);
    int * G = malloc(sizeof(int) * 10000);
    int * B = malloc(sizeof(int) * 10000);

    for(int i = 0; i < 10000; i++){
        R[i] =  255;
        G[i] = 0;
        B[i] = 255;
    }

    create_pgm("test.pgm",nuance,100,100);
    create_ppm("test.ppm",R,G,B,100,100);
    /* On se congratule. */
    return EXIT_SUCCESS;
}