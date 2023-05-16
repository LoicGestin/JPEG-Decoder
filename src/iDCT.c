#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/iDCT.h"

#define pi 3.14159265359

void iDCT(int16_t **phi, uint8_t **S){
    for(int8_t x=0; x<8; x++){

        for(int8_t y=0; y<8; y++){
            float dep = (1 / sqrt(16));
            
            float racine = sqrt(2);
            float t1 =  (float)phi[0][0] / 2;
           
            float t2 = 0.0;
            for (int8_t mu = 1; mu<8; mu++){
                t2 += (float) (cosf(((2*y+1)*mu*pi)/(16)) * (float) phi[0][mu])/racine;
            }

            float t3 = 0.0;
            for (int lambda = 1; lambda<8; lambda++){
                t3 += ((float) cosf(((2*x+1)*lambda*pi)/(16)) *  (float)phi[lambda][0])/racine;
            }

            float t4 = 0.0;
            for (int8_t lambda = 1; lambda<8; lambda++){
                for (int8_t mu = 1; mu<8; mu++){
                    t4 += (float) cosf(((2*y+1)*mu*pi)/(16)) * cosf(((2*x+1)*lambda*pi)/(16)) *  (float)phi[lambda][mu];
                }
            }
      

            float s = dep * (t1 + t2 + t3 + t4);
            S[x][y] = (uint8_t)round(fmaxf(0, fminf(255, s + 128)));
           
        }
    }
}
