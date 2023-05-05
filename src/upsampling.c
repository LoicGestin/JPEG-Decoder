#include <stdio.h>
#include<stdlib.h>

int** sur_ech_h(int ** mat, int l, int c){

    if(l<1 || l>4 || c<1 || c>4){
        return 0;
    }

    int i,j;
    int ** new_mat = (int **)malloc(l * sizeof(int *));

    int new_c = 2*c;

    // multiplication du nombre de colonnes par 2
    for (i=0; i<l; i++){
        new_mat[i] = (int *)malloc(new_c * sizeof(int));
    }

    // remplissage par les valeurs existantes
    for (i=0; i<l; i++){
        for (j=0; j<c; j++){
            new_mat[i][j] = mat[i][j];
        }
    }

    // remplissage du reste de la matrice par des valeurs au hasard
    for (i=0; i<l; i++){
        for(j=c; j<new_c; j++){
            new_mat[i][j] = i+j;
        }
    }

    // Libération mémoire de la matrice d'entrée
    for (i=0; i<l; i++){
        free(mat[i]);
    }
    free(mat);

    return new_mat;
}

int** sur_ech_v(int ** mat, int l, int c){

    if(l<1 || l>4 || c<1 || c>4){
        return 0;
    }

    int i, j;
    int new_l = 2*l;
    int ** new_mat = (int **)malloc(new_l * sizeof(int *));
    
    // multiplication du nombre de lignes par 2
    for (i = 0; i < new_l; i++){
        new_mat[i] = (int *)malloc(c * sizeof(int));
    }
    
    // remplissage par les valeurs existantes
    for (i = 0; i < l; i++){
        for (j = 0; j < c; j++){
            new_mat[i][j] = mat[i][j];
        }
    }
    
    // remplissage du reste de la matrice par des valeurs au hasard
    for (i = l; i < new_l; i++){
        for (j = 0; j < c; j++){
            new_mat[i][j] = i+j;
        }
    }
    
    // Libération mémoire de la matrice d'entrée
    for (i = 0; i < l; i++){
        free(mat[i]);
    }
    free(mat);
    
    return new_mat;
}


// test sur echantillonage horizontal

/*int main() {
    int i, j;
    int l = 2, c = 2;
    int ** mat = (int **)malloc(l * sizeof(int *));
    for (i=0; i<l; i++){
        mat[i] = (int *)malloc(c * sizeof(int));
    }

    // matrice d'entrée
    for (i=0; i<l; i++){
        for(j=0; j<c; j++){
            mat[i][j] = i+j;
        }
    }

    // Test de la fonction sur_ech_h
    int ** new_mat = sur_ech_h(mat, l, c);

    // Affichage de la matrice de sortie
    for (i=0; i<l; i++){
        for(j=0; j<c*2; j++){
            printf("%d ", new_mat[i][j]);
        }
        printf("\n");
    }

    // Libération mémoire de la matrice de sortie
    for (i=0; i<l; i++){
        free(new_mat[i]);
    }
    free(new_mat);

    return 0;
}
*/

// test sur echantillonage verticale

int main() {
    int i, j;
    int l = 2, c = 2;
    int ** mat = (int **)malloc(l * sizeof(int *));
    for (i=0; i<l; i++){
        mat[i] = (int *)malloc(c * sizeof(int));
    }

    // matrice d'entrée
    for (i=0; i<l; i++){
        for(j=0; j<c; j++){
            mat[i][j] = i+j;
        }
    }

    // Test de la fonction sur_ech_v
    int ** new_mat = sur_ech_v(mat, l, c);

    // Affichage de la matrice de sortie
    for (i=0; i<l*2; i++){
        for(j=0; j<c; j++){
            printf("%d ", new_mat[i][j]);
        }
        printf("\n");
    }

    // Libération mémoire de la matrice de sortie
    for (j=0; j<c; j++){
        free(new_mat[j]);
    }
    free(new_mat);

    return 0;
}

