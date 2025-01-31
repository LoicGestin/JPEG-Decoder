#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>


#include "../include/ppm.h"
#include "../include/decode_entete.h"

void create_pgm_header(FILE *file_name, int16_t width, int16_t height) {
    /*Fonction permettant de créer une en-tête d'un fichier pgm à partir d'un nom de fichier
    et des dimensions de l'image*/
    if (file_name == NULL) {
        printf("Erreur dans la création du PGM.\n");
        exit(EXIT_FAILURE);
    }
    // Ecriture des 3 lignes d'en-tête : P5, largeur et hauteur puis 255 (car les valeurs du fichier peuvent aller jusqu'à 255)
    fprintf(file_name, "P5\n");
    fprintf(file_name, "%d %d\n", width, height);
    fprintf(file_name, "255\n");

}

void create_pgm(FILE *file_name, uint8_t ***nuance, int16_t width, int16_t height) {
    /*Fonction permettant de créer un fichier pgm à partir du nom du fichier, des dimensions de l'image et d'un triple pointeur
    vers la nuance*/

    int16_t nb_pixels_w = 0;
    int16_t nb_pixels_h = 0;

    // Ecriture des pixels ligne par ligne si les dimensions de l'image ne sont pas divisibles par 8
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
    // Ecriture des pixels ligne par ligne si les dimensions de l'image sont divisibles par 8
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
    /*Fonction permettant de créer une en-tête d'un fichier ppm à partir d'un nom de fichier
    et des dimensions de l'image*/
    if(file_name == NULL)
    {
        printf("Unable to create ppm file.\n");
        exit(EXIT_FAILURE);
    }
    // Ecriture des 3 lignes d'en-tête : P6, largeur et hauteur puis 255 (car les valeurs du fichier RGB peuvent aller jusqu'à 255)
    fprintf(file_name, "P6\n");
    fprintf(file_name, "%d %d\n", width, height);
    fprintf(file_name, "255\n");

}


void create_ppm(FILE * file_name,  uint8_t *** R,  uint8_t *** G,  uint8_t *** B, struct data *d){
    /*Fonction permettant de créer un fichier ppm en prenant en entrée un pointeur vers le nom du fichier,
    un triple pointeur pour chaque couleur et un pointeur vers notre structure data*/

    int16_t nb_pixels_w = 0;
    int16_t nb_pixels_h = 0;
    int32_t nb_pixels = 0;

    // Déclaration des variables taille de l'image, facteurs d'échantillonnages et listes de composants
    int16_t width = d->image_width;
    int16_t height = d->image_height;
    struct component *comp = d->list_component;
    int8_t sampling_w = comp->sampling_horizontal;
    int8_t sampling_h = comp->sampling_vertical;
    
    int16_t taille_tab_h = width / (8 * sampling_w) + ((width % (8 * sampling_w) != 0) ? 1 : 0);
   
    // Ecriture des pixels ligne par ligne si les dimensions de l'image ne sont pas divisibles par 8
    if (width % 8 != 0 ||  height % 8 != 0 ){
        for (int8_t i = 0; i < 8*sampling_h; i++){
            for (int16_t k = 0; k < taille_tab_h; k++){
                for (int8_t j = 0; j < 8*sampling_w; j++){
                    if(nb_pixels_w >= width || nb_pixels_h >= height){
                        nb_pixels_w++;
                    }
                    else{
                        // pixels stockés dans un tableau de matrice
                        uint8_t red_pixel = R[k][i][j];
                        uint8_t green_pixel = G[k][i][j];
                        uint8_t blue_pixel = B[k][i][j];

                        fputc(red_pixel, file_name);
                        fputc(green_pixel, file_name);
                        fputc(blue_pixel, file_name);
                        nb_pixels_w++;
                        nb_pixels++;
                    }
                }
            }
            nb_pixels_w = 0;
            nb_pixels_h++;
        }
    }
    // Ecriture des pixels ligne par ligne si les dimensions de l'image sont divisibles par 8
    else{
        for (int8_t i = 0; i < 8*sampling_h; i++) {
            for (int16_t k = 0; k < taille_tab_h; k++) {
                for (int8_t j = 0; j < 8*sampling_w; j++){
                    // pixels stockés dans un tableau de matrice
                    uint8_t red_pixel = R[k][i][j];
                    uint8_t green_pixel = G[k][i][j];
                    uint8_t blue_pixel = B[k][i][j];

                    fputc(red_pixel, file_name);
                    fputc(green_pixel, file_name);
                    fputc(blue_pixel, file_name);
                    nb_pixels++;
                }
            }
        }
    }
}
