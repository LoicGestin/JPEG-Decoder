#include <stdio.h>
#include<stdlib.h>

int*** extract_bloc(int prec, int height, int width){
    int nb_blocs_h = height/8;
    int nb_blocs_w = width/8;

    int nb_mat = nb_blocs_h*nb_blocs_w;

    // tableaux de pointeurs
    int*** matrices = malloc(nb_mat * sizeof(int**));

    for(int k=0; k<nb_mat; k++) {
        matrices[k] = malloc(8 * sizeof(int*));
        int** matrice = matrices[k];

        for (int i=0; i<8; i++){
            matrice[i]=malloc(8*sizeof(int));
            for (int j=0; j<8; j++){
                matrice[i][j] = i+j;
            }
        }
    }
    return matrices;
}

int main(){

    int*** matrices = extract_bloc(0, 40, 40);
    int nb_mat = 5*5;
    for(int k = 0; k < nb_mat; k ++){
        int **matrice = matrices[k];
        for(int i=0; i<8; i++){
            for(int j = 0; j < 8; j ++){
                printf("%02d ",matrice[i][j]);
            }
            printf("\n");
        }
    }
        

    return 0;
}