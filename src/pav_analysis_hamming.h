#ifndef PAV_ANALYSIS_HAMMING_H
#define PAV_ANALYSIS_HAMMING_H


float hamming(float *w, unsigned int N);
float compute_power(const float *x, const float *w, unsigned int N, unsigned int M, int trm, float ew);
float compute_am(const float *x, unsigned int N);
float compute_zcr(const float *x, unsigned int N, float fm);

#endif /* PAV_ANALYSIS_HAMMING_H */