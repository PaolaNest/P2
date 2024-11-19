#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "pav_analysis.h"
#include "fic_wave.h"

int main(int argc, char *argv[]) {
    float durTrm = 0.010; //bloques de 10ms
    float fm;
    int   N;
    int   trm;  //Contador nº tramas o ventanas procesadas
    float *x;
    short *buffer; 
    FILE  *fpWave;
    FILE  *fpOut = stdout;  //por defecto la salida será en la consola

    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Empleo: %s inputfile [outputfile]\n", argv[0]);
        return -1;
    }

    // Abrir archivo de entrada WAVE
    if ((fpWave = abre_wave(argv[1], &fm)) == NULL) {
        fprintf(stderr, "Error al abrir el fichero WAVE de entrada %s (%s)\n", argv[1], strerror(errno));
        return -1;
    }

    // Si hay un segundo argumento (archivo de salida), abrirlo
    if (argc == 3) {
        if ((fpOut = fopen(argv[2], "w")) == NULL) {
            fprintf(stderr, "Error al abrir el archivo de salida %s (%s)\n", argv[2], strerror(errno));
            //cierra_wave(fpWave);  // Cerrar archivo WAVE antes de salir
            return -1;
        }
    }

    // Cálculo del tamaño de la ventana
    N = durTrm * fm;
    if ((buffer = malloc(N * sizeof(*buffer))) == 0 || (x = malloc(N * sizeof(*x))) == 0) {
        fprintf(stderr, "Error al ubicar los vectores (%s)\n", strerror(errno));
        cierra_wave(fpWave);
        if (fpOut != stdout) fclose(fpOut);  // Cerrar archivo de salida si está abierto
        return -1;
    }

    trm = 0;
    // Procesar el archivo WAVE por tramas
    while (lee_wave(buffer, sizeof(*buffer), N, fpWave) == N) {

        //Normalizar las muestras dividientdo por 2^15 (16 bits)
        for (int n = 0; n < N; n++) x[n] = buffer[n] / (float) (1 << 15);

        fprintf(fpOut,"%d\t%f\t%f\t%f\n", trm, compute_power(x, N),
                                        compute_am(x, N),
                                        compute_zcr(x, N, fm));
        trm += 1;
    }

    //Liberación de memoria y cierre de archivo:
    cierra_wave(fpWave);
    free(buffer);
    free(x);

    // Si la salida fue en archivo, cerrar el archivo
    if (fpOut != stdout) {
        fclose(fpOut);
    }

    return 0;
}