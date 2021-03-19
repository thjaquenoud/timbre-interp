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
    float **magnitudes;
    float **phases;
};

template <typename Real>
struct polarComplex stft(Real *input, int windowSize, int windowCount, int sampleCount, int slideWindowBy);

template <typename Real>
Real *istft(float **&magnitudes, float **&phases, const int &windowSize, int &windowCount, int sampleCount, const int &slideWindowBy, Real dummy);
