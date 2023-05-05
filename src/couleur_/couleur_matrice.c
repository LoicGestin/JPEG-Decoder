#include <stdlib.h>
#include <stdio.h>

struct Tuple {
    int ** R, G, B;
};

struct Tuple YCbCr_to_RGB(int **Y, int **Cb, int **Cr){
    int **R=malloc(8*sizeof(int *));
    for(int i=0; i<8; i++){
        R[i]=malloc(8*sizeof(int));
    }

    int **G=malloc(8*sizeof(int *));
    for(int i=0; i<8; i++){
        G[i]=malloc(8*sizeof(int));
    }

    int **B=malloc(8*sizeof(int *));
    for(int i=0; i<8; i++){
        B[i]=malloc(8*sizeof(int));
    }

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            R[i][j]= Y[i][j] - 0.0009267 * (Cb[i][j] - 128) + 1.4016868 * (Cr[i][j] - 128);
            G[i][j]= Y[i][j] - 0.3436954 * (Cb[i][j] - 128) + 0.7141690 * (Cr[i][j] - 128);
            B[i][j]= Y[i][j] - 1.7721604 * (Cb[i][j] - 128) + 0.0009902 * (Cr[i][j] - 128);
        }
    }
    struct Tuple tuple= {R, G, B};
    return tuple;
}