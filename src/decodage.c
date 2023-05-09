#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "../include/decode_entete.h"
#include "../include/huffman.h"
#include "../include/extract_block.h"
#include "../include/iDCT.h"
#include "../include/ppm.h"
#include "../include/upsampling.h"
#include "../include/quantification_inverse.h"
#include "../include/zig_zag.h"


int main(int argc, char **argv)
{
    struct data *d = decode_entete("../images/invader.jpg");

    printf("%x \n", d->byte);
    int16_t *block = malloc(sizeof (int16_t) * 64);
    //fread(&d->byte, 1, 1, d->file);
    decode_ac_dc(d,0,1,d->file,block);
    decode_ac_dc(d,0,0,d->file,block);
    printf("\n\n");
    int * data = quantification_inverse(d,0,block);

    for(int i =0; i < 64; i++){
        printf("%hx ", data[i]);
    }
    printf("\n\n");
    int ** matrice = zig_zag(data);
    for(int i =0; i < 8; i++){
        for(int j =0; j < 8; j++){
            printf("%hx ", matrice[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");

    uint8_t **pixel  = malloc(8 * sizeof(uint8_t *));
    for(int i = 0; i < 8; i++) {
        pixel[i] = malloc(8 * sizeof(uint8_t));
    }
    printf("\n");
    for(int i =0; i < 8; i++){
        for(int j =0; j < 8; j++){
            pixel[i][j]= iDCT(i,j,matrice);
            printf("%02x ",pixel[i][j]);
        }
        printf("\n");
    }


    create_pgm("test_invaders.pgm",pixel,8,8);



    return EXIT_SUCCESS;
}