#include "stft.h"

struct polarComplex stft(double *input, unsigned int windowSize, unsigned windowCount, unsigned int sampleCount, unsigned int slideWindowBy) {
    unsigned int windowSizeHalf = windowSize / 2 + 1;
    unsigned int windowCount    = sampleCount / slideWindowBy;
    if ( (windowCount * slideWindowBy) < sampleCount){
        windowCount += 1;
    }
    unsigned int newSampleCount = windowCount * slideWindowBy + ( windowSize - slideWindowBy );

    double *window          = new double[windowSize];
    fftw_complex *fftResult = new fftw_complex[windowSizeHalf];
    fftw_complex *fftWindow = new fftw_complex[windowSizeHalf];
    double *result          = new double[windowSize];

    double **magnitudes    = new double*[windowCount];
    double **phases        = new double*[windowCount];
    double **signalWindows = new double*[windowCount];
    for (int i = 0; i < windowCount; ++i){
        magnitudes[i]    = new double[windowSizeHalf];
        phases[i]        = new double[windowSizeHalf];
        signalWindows[i] = new double[windowSize];
    }

    fftw_plan fftPlan  = fftw_plan_dft_r2c_1d( windowSize, window,    fftResult, FFTW_ESTIMATE );

    // STFT
    for ( int currentWindow = 0; currentWindow < windowCount; ++currentWindow ){
        for (int i = 0; i < windowSize; ++i){
            unsigned int currentSample = currentWindow * slideWindowBy + i;
            if ( ( currentSample ) < 23552 ){
                window[i] = 0.5 * (1 - cos(2 * M_PI * i / windowSize));
            }
            else{
                window[i] = 0.0;
            }
        }

        fftw_execute(fftPlan);

        for (int i = 0; i < windowSizeHalf; ++i){
            magnitudes[currentWindow][i] = sqrt( fftResult[i][0]*fftResult[i][0] + fftResult[i][1]*fftResult[i][1] ) / 2048; // scipy scaling
            phases[currentWindow][i]     = atan2( fftResult[i][1], fftResult[i][0] );
            //std::cout << magnitudes[currentWindow][i] << "\t" << phases[currentWindow][i] << "\n";
        }
    }
    
    struct polarComplex ret = {magnitudes, phases};
    
    return ret;
}

double* istft(double **magnitudes, double **phases, unsigned int windowSize, unsigned windowCount, unsigned int sampleCount, unsigned int slideWindowBy) {
    fftw_complex *fftWindow = new fftw_complex[windowSizeHalf];
    double *result          = new double[windowSize];
    double **signalWindows = new double*[windowCount];
    unsigned int overlap = windowSize - slideWindowBy;
    unsigned int newSampleCount = windowCount * slideWindowBy + overlap;
    double *sampleSignals = new double[newSampleCount];
    *norm = new double[newSampleCount];
    for (int i = 0; i < windowCount; ++i)
        signalWindows[i] = new double[windowSize];

    fftw_plan ifftPlan = fftw_plan_dft_c2r_1d( windowSize, fftWindow, result,    FFTW_ESTIMATE );
    // INVERSE STFT
    for ( int currentWindow = 0; currentWindow < windowCount; ++currentWindow ){
        for ( int i = 0; i < windowSizeHalf; ++i ){
            fftWindow[i][0] = magnitudes[currentWindow][i] * cos( phases[currentWindow][i] );  // Real
            fftWindow[i][1] = magnitudes[currentWindow][i] * sin( phases[currentWindow][i] );  // Imaginary
        }

        fftw_execute(ifftPlan);

        for ( int i = 0; i < windowSize; ++i ){
            signalWindows[currentWindow][i] = result[i] / 2;            // getting normalized result
            signalWindows[currentWindow][i] *= 0.5 * (1 - cos(2 * M_PI * i / windowSize)); // Hann window
        }
    }

    unsigned int pos;

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
        sampleSignals[i] = sampleSignals[i] / norm[i];

    //for (int w = 0; w < newSampleCount; w++)
        //std::cout << sampleSignals[w] << "\t" << norm[w] << "\n";

    return sampleSignalsReshaped;
}
