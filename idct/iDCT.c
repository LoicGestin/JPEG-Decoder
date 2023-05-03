#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>


uint8_t iDCT(int x, int y , int **phi){
    int n = 8;
    
    float dep = (1 / pow(2*n, 1/2));
    float t1 = (1/2) * phi[0][0];

    float t2 = 0.0;
    for (int mu = 1; mu<n; mu++){
        t2 += (1 / pow(2*n, 1/2)) * cosf(((2*x+1)*mu*M_PI)/(2*n)) * phi[0][mu];
    }

    float t3 = 0.0;
    for (int lambda = 1; lambda<n; lambda++){
        t3 += (1 / pow(2*n, 1/2)) * cosf(((2*y+1)*lambda*M_PI)/(2*n)) * phi[lambda][0];
    }

    float t4 = 0.0;
    for (int lambda = 1; lambda<n; lambda++){
        for (int mu = 1; mu<n; mu++){
            t4 += cosf(((2*x+1)*mu*M_PI)/(2*n)) * cosf(((2*y+1)*lambda*M_PI)/(2*n)) * phi[lambda][mu];
        }
    }

    float S = dep * (t1 + t2 + t3 + t4);

    if (S<0.0){
        S=0;
    }
    else if(S>255.0){
        S=255;
    }
    

    S = (uint8_t)(S);


    return S;
}

uint8_t main(){
    int **matrice = malloc(8*sizeof(int*));
    for(int i=0; i<8; i++){
        matrice[i]=malloc(8*sizeof(int));
    }
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            matrice[i][j]=-8;
        }
    }
    int x = 56;
    int y = 58;

    uint8_t S = iDCT(x, y, matrice);
    printf("%d\n", S);

    return 0;
}