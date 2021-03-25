/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MusicAEAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     private juce::Slider::Listener
{
public:
    MusicAEAudioProcessorEditor (MusicAEAudioProcessor&);
    ~MusicAEAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    juce::TextEditor modelTextBox;
    juce::TextButton loadButton, startButton, resetButton;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MusicAEAudioProcessor& audioProcessor;
    
    juce::Slider alpha;
    juce::OwnedArray<juce::Slider> latentSliders;
    
    void reset();
    void sliderValueChanged(juce::Slider* slider) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicAEAudioProcessorEditor)
};
