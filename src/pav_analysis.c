#include <math.h>
#include <stdio.h>
#include "pav_analysis.h"

float compute_power(const float *x, unsigned int N) {
    float pot=1e-12; //para evitar log(0)

    for(unsigned int i = 0; i<N; i++){
        pot += x[i]*x[i];
    }
    return 10*log10(pot/N);
}

float compute_am(const float *x, unsigned int N) {
    float am = 1e-12;
    for(unsigned int i = 0; i<N; i++){
        am += fabs(x[i]);
    }
    return (am/N);
}

//Cruces por zero -> cuando el signo de 2 muestras consecutivas cambia
float compute_zcr(const float *x, unsigned int N, float fm) {
    float ZCR = 0;  
    for (unsigned int i = 1; i < N; i++) {
        if (sgn(x[i]) != sgn(x[i - 1])) {
            ZCR++;
        }
    }
    //return (fm * ZCR) / (2 * (N - 1)); 
    return ZCR/(N-1)*fm/2;
}

int sgn(float x) {
    if( x > 0){
        return 1;
    }else if( x < 0){
        return -1;
    }
    return 0;    
    //return (x == y) ? 1 : 0;
}