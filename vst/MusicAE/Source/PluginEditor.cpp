/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MusicAEAudioProcessorEditor::MusicAEAudioProcessorEditor (MusicAEAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    // these define the parameters of our slider object
    alpha.setSliderStyle (juce::Slider::LinearHorizontal);
    alpha.setRange (0.0, 1.0, 0.01);
    alpha.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    alpha.setPopupDisplayEnabled (true, false, this);
    alpha.setTextValueSuffix (" alpha");
    alpha.setValue(0.5);
 
    // this function adds the slider to the editor
    addAndMakeVisible (&alpha);
        
    for(int i = 0; i < 10; i++){
        auto slider = latentSliders.add(new juce::Slider);
        slider->setSliderStyle (juce::Slider::LinearVertical);
        slider->setRange (0.0, 100.0, 0.01);
        slider->setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
        slider->setPopupDisplayEnabled (true, false, this);
        slider->setTextValueSuffix (" alpha");
        slider->setValue(50.0);
        
        addAndMakeVisible(slider);
    }
    
    addAndMakeVisible(modelTextBox);
    
    startButton.setButtonText("Start");
    addAndMakeVisible(startButton);

    resetButton.setButtonText("Reset");
    addAndMakeVisible(resetButton);

    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 800);
}

MusicAEAudioProcessorEditor::~MusicAEAudioProcessorEditor()
{
}

//==============================================================================
void MusicAEAudioProcessorEditor::paint (juce::Graphics& g)
{
    // fill the whole window white
    g.fillAll (juce::Colours::white);
 
    // set the current drawing colour to black
    g.setColour (juce::Colours::black);
 
    // set the font size and draw text to the screen
    g.setFont (15.0f);
 
    g.drawFittedText ("MusicAE", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void MusicAEAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // sets the position and size of the slider with arguments (x, y, width, height)
    alpha.setBounds (50, 550, 700, 75);
    
    int i = 0;
    for(auto slider : latentSliders){
        slider->setBounds(30 + 80*i, 100, 50, 400);
        i++;
    }
    
    modelTextBox.setBounds(100, 650, 200, 100);
    startButton.setBounds(400, 650, 100, 100);
    resetButton.setBounds(600, 650, 100, 100);
}

void MusicAEAudioProcessorEditor::reset()
{
    for(auto slider : latentSliders)
        slider->setValue(50.0);
}

