#include <stdio.h>
#include <stdint.h>  // Para utilizar uint32_t
#include "fic_wave.h"

FILE *abre_wave(const char *ficWave, float *fm) {
    FILE *fpWave;
    uint32_t sampleRate;

    // Abre archivo WAVE en modo lectura binaria ("rb")
    if ((fpWave = fopen(ficWave, "rb")) == NULL) return NULL;

    // Del byte 24 al 27 se encuentra la frec de muestreo
    if (fseek(fpWave, 24, SEEK_SET) < 0) return NULL;

    // Lee los 4 bytes que contienen la frec de muestreo
    if (fread(&sampleRate, sizeof(uint32_t), 1, fpWave) != 1) {
        fclose(fpWave);
        return NULL;
    }

    // Convertir frec de muestreo de uint32_t a float
    *fm = (float)sampleRate;

    // Offset de 44 bytes -> datos de audio
    if (fseek(fpWave, 44, SEEK_SET) < 0) {
        fclose(fpWave);
        return NULL;
    }

    return fpWave;
}

size_t lee_wave(void *x, size_t size, size_t nmemb, FILE *fpWave) {
    return fread(x, size, nmemb, fpWave);
}

void cierra_wave(FILE *fpWave) {
    fclose(fpWave);
}