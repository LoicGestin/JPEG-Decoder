#include <stdio.h>
#include <stdlib.h>

int* YCbCr_to_RGB(int Y, int Cb, int Cr){
    int *tab = (int*) malloc(sizeof(int) * 3);

    *tab = Y - 0.0009267 * (Cb - 128) + 1.4016868 * (Cr - 128);
    *(tab + 1) = Y - 0.3436954 * (Cb - 128) + 0.7141690 * (Cr - 128);
    *(tab + 2) = Y - 1.7721604 * (Cb - 128) + 0.0009902 * (Cr - 128);

    return tab;
}

int main(){
    int val_Y = 150;
    int val_Cb = 150;
    int val_Cr = 150;

    int *arr = YCbCr_to_RGB(val_Y, val_Cb, val_Cr);
    int R = arr[0];
    int G = arr[1];
    int B = arr[2];

    printf("R = %d, G = %d, B = %d\n", R, G, B);

    return 0;
}