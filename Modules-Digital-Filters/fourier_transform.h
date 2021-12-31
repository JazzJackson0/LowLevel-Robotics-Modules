#pragma once
#include <complex.h>

typedef struct binData frequencyBin_data;
typedef struct sampleData IDFT_sample_data;

//Functions
frequencyBin_data* fourier_transform(float sample_values[], int const samples_per_sec);
IDFT_sample_data* IDFT(float frequency_values[], int const samples_per_sec);

//Structs
struct binData {
    //Fourier Coefficients
    float realPart;
    float complexPart;

    float nyquist_magnitude;
    float amplitude;
    float phase; //Ignoring Phase for now...
};

struct sampleData {
    float realPart;
    float complexPart;
};
