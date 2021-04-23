/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <cstdio>
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioGenerator.h"

//==============================================================================
/**
*/
class MusicAEAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MusicAEAudioProcessor();
    ~MusicAEAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    //void processBlock (juce::AudioBuffer<double>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void reset();
    void updateState(enum MusicAE_state new_state);
    
    bool process {false};
    bool genInit {false};
    enum MusicAE_state state {STATE_SYNTH};
    float temp_alpha;
    std::vector<float> temp_sliders;

    AudioGenerator *generator;

private:
    juce::AudioBuffer<float> fltDelayBuffer;
    juce::AudioBuffer<double> dblDelayBuffer;
    juce::AudioBuffer<float> fltFadeBuffer;
    juce::AudioBuffer<double> dblFadeBuffer;
    juce::AudioBuffer<float> tempAudioBuffer[2]; // for temporary usage while we test on .wav files
    int tempAudioBufferReadIndex[2] {0, 0};
    int delayBufferReadIndex {0};
    int delayBufferWriteIndex {0};
    int delayBufferProcessCounter {0};
    double sampRate {44100};
    int batches {20};
    int overlap {0};
    const int chunk {1024};

    template <typename Real>
    juce::AudioBuffer<Real> processSamples(juce::AudioBuffer<Real>& buffer, juce::MidiBuffer& midiMessages, juce::AudioBuffer<Real>& delayBuffer, juce::AudioBuffer<Real>& fadeBuffer);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicAEAudioProcessor)
};
