#include <math.h>
#include <stdio.h>
#include "pav_analysis.h"

// Cálculo de la potencia
float compute_power(const float *x, unsigned int N) {
    float pot = 1e-12; // para evitar log de 0
    for (unsigned int i = 0; i < N; i++) {
        pot += x[i] * x[i];
    }
    return 10 * log10(pot / N);
}

// Cálculo de la amplitud media
float compute_am(const float *x, unsigned int N) {
    float amplitud = 1e-12;
    for (unsigned int i = 0; i < N; i++) {
        amplitud += fabs(x[i]);
    }
    return amplitud / N;
}

// Cálculo de la tasa de cruces por cero (ZCR)
float compute_zcr(const float *x, unsigned int N, float fm) {
    float ZCR = 0;  // Inicializamos en 0 porque es un contador
    for (unsigned int i = 1; i < N; i++) {
        if (sign(x[i]) != sign(x[i - 1])) {
            ZCR++;  // Se cuenta cuando hay un cambio de signo (cruce por cero)
        }
    }
    return (fm * ZCR) / (2 * (N - 1));  // Fórmula estándar de ZCR
}

// Función delta para calcular los cruces por cero
int sign(float x) {
    if (x > 0) {
        return 1;
    } else if (x < 0) {
        return -1;
    }
    return 0;  // Si es exactamente 0, consideramos 0
}