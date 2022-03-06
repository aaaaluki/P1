#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fic_wave.h"

static WaveHeader_t* pwaveHeader;

FILE *abre_wave(const char *ficWave, float *fm) {
    FILE *fpWave;

    // Comprobar que el fichero se puede abrir y tiene cabecera
    if ((fpWave = fopen(ficWave, "r")) == NULL) return NULL;
    if (fseek(fpWave, 44, SEEK_SET) < 0) return NULL;

    // Volver el puntero al origen
    fseek(fpWave, 0, SEEK_SET);

    // Leer la cabecera WAVE
    pwaveHeader = (WaveHeader_t *) malloc(sizeof(WaveHeader_t));
    uint8_t buff4[4];
    uint8_t buff2[2];

    // Leer el chunk RIFF
    fread(pwaveHeader->chunkID, sizeof(pwaveHeader->chunkID), 1, fpWave);
    fread(buff4, 4, 1, fpWave);
    pwaveHeader->chunk_size = ARR2UINT32(buff4);
    fread(pwaveHeader->format, 4, 1, fpWave);

    // Leer el sub-chunk fmt
    fread(pwaveHeader->subchunk1ID, 4, 1, fpWave);
    fread(buff4, 4, 1, fpWave);
    pwaveHeader->subchunk1Size = ARR2UINT32(buff4);
    fread(buff2, 2, 1, fpWave);
    pwaveHeader->audioFormat = ARR2INT16(buff2);
    fread(buff2, 2, 1, fpWave);
    pwaveHeader->numChannels = ARR2INT16(buff2);
    fread(buff4, 4, 1, fpWave);
    pwaveHeader->sampleRate = ARR2UINT32(buff4);
    fread(buff4, 4, 1, fpWave);
    pwaveHeader->byteRate = ARR2UINT32(buff4);
    fread(buff2, 2, 1, fpWave);
    pwaveHeader->blockAlign = ARR2INT16(buff2);
    fread(buff2, 2, 1, fpWave);
    pwaveHeader->bpSample = ARR2INT16(buff2);

    // Leer el sub-chunk data
    fread(pwaveHeader->subchunk2ID, 4, 1, fpWave);
    fread(buff4, 4, 1, fpWave);
    pwaveHeader->subchunk2Size = ARR2UINT32(buff4);

    // Mostrar cabecera wave
    printHeader(pwaveHeader, ficWave);

    // Poner la frecuencia de muestreo
    *fm = (float) pwaveHeader->sampleRate;

    // Comprobar que la señal es como la que pide el ejercicio 1 de la
    // ampliación
    // Señal mono o estereo
    if (pwaveHeader->numChannels > 2 || pwaveHeader->numChannels < 0) {
        fprintf(stderr, "[ERROR] El fichero debe ser estereo o mono\n");
        return NULL;
    }

    // PCM Lineal 16 bits
    // PCM || Lineal || 16 bits
    if (pwaveHeader->subchunk1Size != 16 || pwaveHeader->audioFormat != 1
        || pwaveHeader->bpSample != 16) {
        fprintf(stderr, "[ERROR] El fichero debe ser estar codificado con PCM Lineal de 16 bits\n");
        return NULL;
    }

    return fpWave;
}

size_t lee_wave(void *x, size_t size, size_t nmemb, FILE *fpWave) {
    if (pwaveHeader->numChannels == 1) {
        return fread(x, size, nmemb, fpWave);
    } else {
        size_t elemsRead;
        short *buffStereo = (short *) malloc(2*nmemb * sizeof(short));
        short tmp;

        elemsRead = fread(buffStereo, sizeof(*buffStereo), 2*nmemb, fpWave);

        for (int i = 0; i < nmemb; i++) {
            tmp = buffStereo[2*i]/2 + buffStereo[2*i + 1]/2;
            // memcpy hace lo mismo que: x[i] = tmp
            memcpy(x + i*sizeof(short), &tmp, sizeof(short));
        }

        free(buffStereo);
        return elemsRead / 2;
    }
}

void cierra_wave(FILE *fpWave) {
    fclose(fpWave);
    free(pwaveHeader);
}

void printHeader(WaveHeader_t *phead, const char *ficWave) {
    printf("File Name       : %s\n", ficWave);

    // Chunk RIFF
    printf("Chunk ID        : %.4s\n", phead->chunkID);
    printf("Chunk Size      : %u bytes\n", phead->chunk_size);
    printf("Format          : %.4s\n", phead->format);
    
    // Sub-chunk fmt
    printf("Subchunk1 ID    : %.4s\n", phead->subchunk1ID);
    printf("Subchunk1 Size  : %u bytes\n", phead->subchunk1Size);
    printf("Audio Format    : %d\n", phead->audioFormat);
    printf("Num channels    : %d\n", phead->numChannels);
    printf("Sample rate     : %u Hz\n", phead->sampleRate);
    printf("Byte rate       : %u bps\n", phead->byteRate);
    printf("Block Align     : %d\n", phead->blockAlign);
    printf("BitsPerSample   : %d bits\n", phead->bpSample);

    // Sub-chunk data
    printf("Subchunk2 ID    : %.4s\n", phead->subchunk2ID);
    printf("Subchunk2 Size  : %u bytes\n", phead->subchunk2Size);

    // Algunos extras
    printf("Duration        : %.2f s\n", (float)phead->subchunk2Size / phead->byteRate);
}
