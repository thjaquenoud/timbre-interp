#include "stft.h"

template <typename Real>
struct polarComplex stft(Real *input, int windowSize, int windowCount, int sampleCount, int slideWindowBy) {
    int windowSizeHalf = windowSize / 2 + 1;
    if ( (windowCount * slideWindowBy) < sampleCount){
        windowCount += 1;
    }
    int newSampleCount = windowCount * slideWindowBy + ( windowSize - slideWindowBy );

    float *window          = new float[windowSize];
    fftwf_complex *fftResult = new fftwf_complex[windowSizeHalf];
    fftwf_complex *fftWindow = new fftwf_complex[windowSizeHalf];
    float *result          = new float[windowSize];

    Real **magnitudes    = new float*[windowCount];
    Real **phases        = new float*[windowCount];
    Real **signalWindows = new float*[windowCount];
    for (int i = 0; i < windowCount; ++i){
        magnitudes[i]    = new float[windowSizeHalf];
        phases[i]        = new float[windowSizeHalf];
        signalWindows[i] = new float[windowSize];
    }

    fftwf_plan fftPlan  = fftwf_plan_dft_r2c_1d( windowSize, window,    fftResult, FFTW_ESTIMATE );

    // STFT
    for ( int currentWindow = 0; currentWindow < windowCount; ++currentWindow ){
        for (int i = 0; i < windowSize; ++i){
            int currentSample = currentWindow * slideWindowBy + i;
            if ( ( currentSample ) < 23552 ){
                window[i] = 0.5 * (1 - cos(2 * M_PI * i / windowSize)) * (float)input[currentSample];
            }
            else{
                window[i] = 0.0;
            }
        }

        fftwf_execute(fftPlan);

        for (int i = 0; i < windowSizeHalf; ++i){
            magnitudes[currentWindow][i] = sqrt( fftResult[i][0]*fftResult[i][0] + fftResult[i][1]*fftResult[i][1] ) / 2048; // scipy scaling
            phases[currentWindow][i]     = atan2( fftResult[i][1], fftResult[i][0] );
            //std::cout << magnitudes[currentWindow][i] << "\t" << phases[currentWindow][i] << "\n";
        }
    }
    
    struct polarComplex ret = {magnitudes, phases};
    
    return ret;
}

template <typename Real>
Real *istft(float **&magnitudes, float **&phases, const int &windowSize, int &windowCount, int sampleCount, const int &slideWindowBy, Real dummy) {
    int windowSizeHalf = windowSize / 2 + 1;
    fftwf_complex *fftWindow = new fftwf_complex[windowSizeHalf];
    float *result          = new float[windowSize];
    float **signalWindows = new float*[windowCount];
    int overlap = windowSize - slideWindowBy;
    int newSampleCount = windowCount * slideWindowBy + overlap;
    float *sampleSignals = new float[newSampleCount];
    float *norm = new float[newSampleCount];
    Real *sampleSignalsNew = new Real[newSampleCount];
    for (int i = 0; i < windowCount; ++i)
        signalWindows[i] = new float[windowSize];

    fftwf_plan ifftPlan = fftwf_plan_dft_c2r_1d( windowSize, fftWindow, result,    FFTW_ESTIMATE );
    // INVERSE STFT
    for ( int currentWindow = 0; currentWindow < windowCount; ++currentWindow ){
        for ( int i = 0; i < windowSizeHalf; ++i ){
            fftWindow[i][0] = magnitudes[currentWindow][i] * cos( phases[currentWindow][i] );  // Real
            fftWindow[i][1] = magnitudes[currentWindow][i] * sin( phases[currentWindow][i] );  // Imaginary
        }

        fftwf_execute(ifftPlan);

        for ( int i = 0; i < windowSize; ++i ){
            signalWindows[currentWindow][i] = result[i] / 2;            // getting normalized result
            signalWindows[currentWindow][i] *= 0.5 * (1 - cos(2 * M_PI * i / windowSize)); // Hann window
        }
    }

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
    for (int i = 0; i < newSampleCount; i++)
        sampleSignalsNew[i] = (Real)(sampleSignals[i] / norm[i]);

    //for (int w = 0; w < newSampleCount; w++)
        //std::cout << sampleSignals[w] << "\t" << norm[w] << "\n";

    return sampleSignalsNew;
}
