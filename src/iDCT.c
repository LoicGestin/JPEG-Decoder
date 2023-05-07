#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>


uint8_t **iDCT(int **phi){
    uint8_t **S=malloc(8*sizeof(uint8_t*));
    for(int x=0; x<8; x++){
        S[x]=malloc(8*sizeof(uint8_t));
        for(int y=0; y<8; y++){
            float dep = (1 / pow(2*8, 1/2));
            float t1 = (1/2) * phi[0][0];

            float t2 = 0.0;
            for (int mu = 1; mu<8; mu++){
                t2 += (1 / pow(2, 1/2)) * cosf(((2*y+1)*mu*M_PI)/(2*8)) * phi[0][mu];
            }

            float t3 = 0.0;
            for (int lambda = 1; lambda<8; lambda++){
                t3 += (1 / pow(2, 1/2)) * cosf(((2*x+1)*lambda*M_PI)/(2*8)) * phi[lambda][0];
            }

            float t4 = 0.0;
            for (int lambda = 1; lambda<8; lambda++){
                for (int mu = 1; mu<8; mu++){
                    t4 += cosf(((2*y+1)*mu*M_PI)/(2*8)) * cosf(((2*x+1)*lambda*M_PI)/(2*8)) * phi[lambda][mu];
                }
            }

            float s = dep * (t1 + t2 + t3 + t4);
            if (s<0.0){
                s=0;
            }
            else if(s>255.0){
                s=255;
            }
    

            S[x][y] = (uint8_t)(s);
        }
    }
    return S;
}
