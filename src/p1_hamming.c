#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fic_wave.h"
#include "pav_analysis_hamming.h"

int main(int argc, char *argv[]) {
    float durTrm = 0.010;
    float Tlong = 0.020;
    float Tdesp = 0.010;
    float fm, wEg;
    int   N, M, trm;
    float *x, *w;
    short *buffer;
    FILE  *fpWave;
    FILE  *fpOut = stdout;

    switch (argc) {
    case 3:
        if ((fpOut = fopen(argv[2], "w")) == NULL ) {
            fprintf(stderr, "Error al abrir el archivo de salida %s (%s)\n",
                    argv[2], strerror(errno));
            return -1;
        }
        // El break no esta a proposito, asi despues de abrir el fichero de
        // salida intentara abrir el de entrada

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

    N = fm * Tlong;
    M = fm * Tdesp;
    
    // Con calloc el buffer queda inicializado con ceros
    if ((buffer = calloc(N, sizeof(*buffer))) == 0 ||
        (x = malloc(N * sizeof(*x))) == 0 ||
        (w = malloc(N * sizeof(*w))) == 0) {
        fprintf(stderr, "Error al ubicar los vectores (%s)\n",
                strerror(errno));
        return -1;
    }

    wEg = hamming(w, N);

    // mientras leamos M elementos
    trm = 0;
    short aux;
    while (lee_wave(buffer, sizeof(*buffer), M, fpWave) == M) {
        for (int n=M, i=0; n < N; n++, i++) {
            aux = buffer[i];
            buffer[i] = buffer[n];  // ponemos al principio las N-M nuestras del final del buffer
            buffer[n] = aux;        // ponemos al final del buffer las muestras leidas
        }

        for (int n = M; n < N; n++) {
            x[n] = (float) buffer[n] / (1 << 15); // ahora normalizamos valores del buffer y los asignamos a x
        }

        fprintf(fpOut, "%d\t%f\t%f\t%f\n", trm, compute_power(x,w,N,wEg),
                compute_am(x,N), compute_zcr(x,N,fm));
        trm += 1;
    }

    cierra_wave(fpWave);
    fclose(fpOut);
    free(buffer);
    free(x);
    free(w);

    return 0;
}
