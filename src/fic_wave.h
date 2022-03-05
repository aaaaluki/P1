#ifndef FIC_WAVE_H
#define FIC_WAVE_H

#include <stdint.h>

#define ARR2UINT32(a) (a[3] << 24) + (a[2] << 16) + (a[1] << 8) + a[0]
#define ARR2INT16(a) (a[1] << 8) + a[0]

typedef struct {
    // Source: http://soundfile.sapp.org/doc/WaveFormat/

    // RIFF Chunk descriptor
    uint8_t chunkID[4];
    uint32_t chunk_size;
    uint8_t format[4];

    // fmt sub-chunk
    uint8_t subchunk1ID[4];
    uint32_t subchunk1Size;
    int16_t audioFormat;
    int16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    int16_t blockAlign;
    int16_t bpSample;

    // data sub-chunk
    uint8_t subchunk2ID[4];
    uint32_t subchunk2Size;

} WaveHeader_t;


FILE   *abre_wave(const char *ficWave, float *fm);
size_t lee_wave(void *x, size_t size, size_t nmemb, FILE *fpWave);
void   cierra_wave(FILE *fpWave);

void printHeader(WaveHeader_t *phead, const char *ficWave);

#endif	/* FIC_WAVE_H	*/
