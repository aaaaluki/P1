#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "pav_analysis.h"
#include "fic_wave.h"


int main(int argc, char *argv[]) {
    float durTrm = 0.010;
    float fm;
    int N, trm;
    float *x;
    short *buffer;
    FILE *fpWave;
    FILE *fpOut = stdout;

    switch (argc) {
    case 3:
        if ((fpOut = fopen(argv[2], "w")) == NULL) {
            fprintf(stderr, "Error al abrir el archivo de salida %s (%s)\n",
                    argv[2], strerror(errno));
            return -1;
        }
        // El break no esta a proposito, asi despues de abrir el de salida
        // intentara abrir el de entrada

    case 2:
        if ((fpWave = abre_wave(argv[1], &fm)) == NULL) {
            fprintf(stderr, "Error al abrir el fichero WAVE de entrada %s (%s)\n",
                    argv[1], strerror(errno));
            return -1;
        }
        break;
    
    default:
        fprintf(stderr, "Empleo: %s inputfile [outputfile]\n", argv[0]);
        return -1;
    }
    
    N = durTrm * fm;
    if ((buffer = malloc(N * sizeof(*buffer))) == 0 ||
        (x = malloc(N * sizeof(*x))) == 0) {
        fprintf(stderr, "Error al ubicar los vectores (%s)\n",
                strerror(errno));
        return -1;
    }

    // mientras leamos N elementos
    trm = 0;
    while (lee_wave(buffer, sizeof(*buffer), N, fpWave) == N) {
        for (int n = 0; n < N; n++) x[n] = (float)buffer[n] / (1 << 15);

        fprintf(fpOut, "%d\t%f\t%f\t%f\n", trm, compute_power(x,N),
                compute_am(x,N), compute_zcr(x,N,fm));
        trm += 1;
    }

    cierra_wave(fpWave);
    fclose(fpOut);
    free(buffer);
    free(x);

    return 0;
}
