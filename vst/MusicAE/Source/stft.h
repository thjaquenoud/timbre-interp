/*
  ==============================================================================

    stft.h
    Created: 8 Mar 2021 5:24:59pm
    Author:  sam

  ==============================================================================
*/

#pragma once
#include <fftw3.h>
#include <cmath>
#include <iostream>

struct polarComplex{
    double **magnitudes;
    double **phases;
};

struct polarComplex stft(double *input, unsigned int windowSize, unsigned windowCount, unsigned int sampleCount, unsigned int slideWindowBy);
double* istft(double **magnitudes, double **phases, unsigned int windowSize, unsigned windowCount, unsigned int sampleCount, unsigned int slideWindowBy);
