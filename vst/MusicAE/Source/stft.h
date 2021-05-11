/*
  ==============================================================================

    stft.h
    Created: 8 Mar 2021 5:24:59pm
    Author:  MusicAE

  ==============================================================================
*/

#pragma once
#include <fftw3.h>
#include <cmath>
#include <iostream>

struct stftReturn{
    tensorflow::Tensor magnitudes;
    float **phases;
    float *max;
};

template <typename Real>
struct stftReturn stft(Real *input, Real *excessInput, int excess, int windowSize, int windowCount, int sampleCount, int slideWindowBy) {
    int mainInputEnd = sampleCount - excess;
    int windowSizeHalf = windowSize / 2 + 1;
    if ( (windowCount * slideWindowBy) < sampleCount){
        windowCount += 1;
    }

    float *window          = new float[windowSize];
    fftwf_complex *fftResult = new fftwf_complex[windowSizeHalf];
    float *max             = new float[windowCount];

    float **magnitudes    = new float*[windowCount];
    float **phases        = new float*[windowCount];
    for (int i = 0; i < windowCount; ++i){
        magnitudes[i]    = new float[windowSizeHalf];
        phases[i]        = new float[windowSizeHalf];
    }

    fftwf_plan fftPlan  = fftwf_plan_dft_r2c_1d( windowSize, window,    fftResult, FFTW_ESTIMATE );

    tensorflow::Tensor magnitudes_tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({windowCount, windowSizeHalf}));
    
    // STFT
    for ( int currentWindow = 0; currentWindow < windowCount; ++currentWindow ){
        for (int i = 0; i < windowSize; ++i){
            int currentSample = currentWindow * slideWindowBy + i;
            if ( ( currentSample ) < sampleCount ){
                if (currentSample < mainInputEnd)
                    window[i] = 0.5 * (1 - cos(2 * M_PI * i / windowSize)) * (float)input[currentSample];
                else
                    window[i] = 0.5 * (1 - cos(2 * M_PI * i / windowSize)) * (float)excessInput[currentSample - mainInputEnd];
            }
            else{
                window[i] = 0.0;
            }
        }

        fftwf_execute(fftPlan);

        max[currentWindow] = 0;
        for (int i = 0; i < windowSizeHalf; ++i){
            magnitudes[currentWindow][i] = sqrt( fftResult[i][0]*fftResult[i][0] + fftResult[i][1]*fftResult[i][1] ) / 2048; // scipy scaling
            phases[currentWindow][i]     = atan2( fftResult[i][1], fftResult[i][0] );
            if (magnitudes[currentWindow][i] > max[currentWindow])
                max[currentWindow] = magnitudes[currentWindow][i];
        }
        
        max[currentWindow] += 1e-9;
        for (int i = 0; i < windowSizeHalf; ++i){
            magnitudes_tensor.flat<float>()(windowSizeHalf * currentWindow + i) = magnitudes[currentWindow][i] / max[currentWindow];
        }
    }
    
    struct stftReturn ret = {magnitudes_tensor, phases, max};
    
    delete window;
    delete fftResult;
    for (int i = 0; i < windowCount; i++)
        delete magnitudes[i];
    delete magnitudes;
    
    return ret;
}

template <typename Real>
Real* istft(float **&magnitudes, float **&phases, const int &windowSize, int &windowCount, int sampleCount, const int &slideWindowBy, const Real *fade, bool first, bool synth) {
    int windowSizeHalf = windowSize / 2 + 1;
    fftwf_complex *fftWindow = new fftwf_complex[windowSizeHalf];
    float *result          = new float[windowSize];
    float **signalWindows = new float*[windowCount];
    int overlap = windowSize - slideWindowBy;
    int newSampleEnd = windowCount * slideWindowBy;
    if (!synth)
        newSampleEnd += slideWindowBy;
    float *sampleSignals = new float[newSampleEnd + overlap];
    float *norm = new float[newSampleEnd + overlap];
    Real *newSampleSignals = new Real[newSampleEnd - overlap];
    for (int i = 0; i < windowCount; ++i)
        signalWindows[i] = new float[windowSize];

    fftwf_plan ifftPlan = fftwf_plan_dft_c2r_1d( windowSize, fftWindow, result,    FFTW_ESTIMATE );
    // INVERSE STFT
    for ( int currentWindow = 0; currentWindow < windowCount; ++currentWindow ){
        for ( int i = 0; i < windowSizeHalf; ++i ){
            fftWindow[i][0] = magnitudes[currentWindow][i] * cos( phases[currentWindow][i] );  // Real
            fftWindow[i][1] = magnitudes[currentWindow][i] * sin( phases[currentWindow][i] );  // Imaginary
        }
        delete magnitudes[currentWindow];
        delete phases[currentWindow];

        fftwf_execute(ifftPlan);

        for ( int i = 0; i < windowSize; ++i ){
            signalWindows[currentWindow][i] = result[i] / 2;            // getting normalized result
            signalWindows[currentWindow][i] *= 0.5 * (1 - cos(2 * M_PI * i / windowSize)); // Hann window
        }
    }
    delete magnitudes;
    delete phases;

    int pos;

    // HERE WE COMBINE RESULTED WINDOWS

    // COMBINE AND AVERAGE
    // 1st window should be full replace
    for ( int i = 0; i < windowSize; ++i ){
        sampleSignals[i] = signalWindows[0][i];
        norm[i] = pow(0.5 * (1 - cos(2 * M_PI * i / windowSize)), 2);
    }
    // 2nd window and onwards: combine with previous ones
    for ( int currentWindow = 1; currentWindow < windowCount; ++currentWindow ){
        // combine and average with data from previous window
        for ( int i = 0; i < (windowSize - slideWindowBy); ++i ){
            pos = currentWindow * slideWindowBy + i;
            sampleSignals[pos] += signalWindows[currentWindow][i];
            norm[pos] += pow(0.5 * (1 - cos(2 * M_PI * i / windowSize)), 2);
        }
        // simply replace for the rest
        for ( int i = (windowSize - slideWindowBy); i < windowSize; ++i ){
            pos = currentWindow * slideWindowBy + i;
            sampleSignals[pos] = signalWindows[currentWindow][i];
            norm[pos] = pow(0.5 * (1 - cos(2 * M_PI * i / windowSize)), 2);
        }
    }

    norm[0] = 1e-16;
    for (int i = overlap; i < newSampleEnd; i++){
        int j = i - overlap;
        newSampleSignals[j] = (Real)(sampleSignals[i] / norm[i]);
        if (j < slideWindowBy && !first && !synth){
            newSampleSignals[j] = (Real)(((float)j / (float)slideWindowBy) * newSampleSignals[j] + ((float)(slideWindowBy - j) / (float)slideWindowBy) * fade[j]);
        }
    }
                
    delete fftWindow;
    delete result;
    for (int i = 0; i < windowCount; i++)
        delete signalWindows[i];
    delete signalWindows;
    delete sampleSignals;
    delete norm;

    return newSampleSignals;
}
