#include <stdio.h>

struct Tuple {
    int R, G, B;
};

struct Tuple YCbCr_to_RGB(int Y, int Cb, int Cr){
    float R = Y - 0.0009267 * (Cb - 128) + 1.4016868 * (Cr - 128);
    float G = Y - 0.3436954 * (Cb - 128) + 0.7141690 * (Cr - 128);
    float B = Y - 1.7721604 * (Cb - 128) + 0.0009902 * (Cr - 128);

    R = (int)(R);
    G = (int)(G);
    B = (int)(B);

    struct Tuple tuple = {R, G, B};

    return tuple;
}

int main(){
    int val_Y = 150;
    int val_Cb = 150;
    int val_Cr = 150;

    struct Tuple tuple = YCbCr_to_RGB(val_Y, val_Cb, val_Cr);

    int R = tuple.R;
    int G = tuple.G;
    int B = tuple.B;

    printf("R = %d, G = %d, B = %d\n", R,G,B);

    return 0;
}

