#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "../include/decode_entete.h"
#include "../include/huffman.h"
#include "../include/extract_bloc.h"
#include "../include/iDCT.h"
#include "../include/ppm.h"
#include "../include/upsampling.h"
#include "../include/quantification_inverse.h"
#include "../include/zig_zag.h"


int main(int argc, char **argv)
{
    struct data *d = decode_entete("../images/invader.jpeg");

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

      
    printf("\n");
    uint8_t **pixel= iDCT(matrice);


    for(int i =0; i < 8; i++){
        for(int j =0; j < 8; j++){
            printf("%02x ", pixel[i][j]);
        }
        printf("\n");
    }

    create_pgm("test_invaders.pgm",pixel,8,8);



    return EXIT_SUCCESS;
}