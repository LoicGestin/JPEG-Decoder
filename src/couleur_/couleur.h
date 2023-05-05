#ifndef _COULEUR_H_
#define _COULEUR_H_

struct Tuple;

struct Tuple YCbCr_to_RGB(int **Y, int **Cb, int **Cr);

#endif /*_COULEUR_H_*/