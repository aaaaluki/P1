#include <stdio.h>
#include <math.h>
#include "pav_analysis_hamming.h"

/**
 * Funcion signo, definida de la siguiente manera:
 *      1 : x > 0
 *      0 : x = 0
 *     -1 : x < 0
 */
#define SGN(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))

float hamming(float *w, unsigned int N) {
    // Generamos la ventana de hamming
    float energy = 0.0f;
    for (int i = 0; i < N; i++) {
        w[i] = 0.54 - (0.46 * cosf((2 * M_PI * i) / N));
        energy += w[i]*w[i];
    }

    return energy;
}

float compute_power(const float *x, const float *w, unsigned int N, float ew) {
    // Valor para que la pot. mínima sea -120 dB (consejo para la P2)
    float power = 1.e-12;

    // Calculamos los sumatorios
    for (int j = 0; j < N; j++) {
        power += (x[j] * w[j]) * (x[j] * w[j]);
    }
    
    // Potencia media en dB
    return 10 * log10f(power / ew);
}

float compute_am(const float *x, unsigned int N) {
    float a = 0.0f;

    for (int i = 0; i < N; i++) {
        a += fabsf(x[i]);
    }

    return a / N;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
    const float scaling = 0.5f * fm / (N - 1);
    float zcr = 0.0f;

    for (int i = 1; i < N; i++) {
        if (SGN(x[i]) != SGN(x[i - 1])) {
            zcr++;
        }
	}

    return scaling*zcr;
}
