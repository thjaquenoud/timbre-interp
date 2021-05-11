/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

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
    alpha.addListener(this);
    alpha.setLookAndFeel(&sliderLookAndFeel);
 
    // this function adds the slider to the editor
    addAndMakeVisible (&alpha);
    alpha.setVisible(false);
        
    for(int i = 0; i < 10; i++){
        auto slider = latentSliders.add(new juce::Slider);
        slider->setSliderStyle (juce::Slider::LinearVertical);
        slider->setRange (0.0, 100.0, 0.01);
        slider->setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
        slider->setPopupDisplayEnabled (true, false, this);
        slider->setTextValueSuffix (" latent " + std::to_string(i+1));
        slider->setValue(50.0);
        slider->addListener(this);
        slider->setLookAndFeel(&sliderLookAndFeel);
        
        addAndMakeVisible(slider);
    }
    
    addAndMakeVisible(synthButton);
    synthButton.setEnabled(false);
    synthButton.setLookAndFeel(&buttonLookAndFeel);
    synthButton.onClick = [this]()
    {
        this->onStateChange(STATE_SYNTH);
    };
    
    addAndMakeVisible(effectsButton);
    effectsButton.setEnabled(true);
    effectsButton.setLookAndFeel(&buttonLookAndFeel);
    effectsButton.onClick = [this]()
    {
        this->onStateChange(STATE_EFFECTS);
    };
    
    addAndMakeVisible(mixerButton);
    mixerButton.setEnabled(true);
    mixerButton.setLookAndFeel(&buttonLookAndFeel);
    mixerButton.onClick = [this]()
    {
        this->onStateChange(STATE_MIXER);
    };
    
    synthButton.setRadioGroupId(StateButtons);
    effectsButton.setRadioGroupId(StateButtons);
    mixerButton.setRadioGroupId(StateButtons);
    
    modelTextBox.setMultiLine(false);
    modelTextBox.setTextToShowWhenEmpty("Model name", juce::Colour((juce::uint8)255, (juce::uint8)255, (juce::uint8)255, 0.3f));
    addAndMakeVisible(modelTextBox);

    loadButton.setLookAndFeel(&buttonLookAndFeel);
    addAndMakeVisible(loadButton);
    loadButton.onClick = [this]()
    {
        this->audioProcessor.generator->model_name = this->modelTextBox.getText().toStdString();
        this->audioProcessor.generator->modelToMem();
        this->startButton.setEnabled(true);
    };

    addAndMakeVisible(startButton);
    startButton.setEnabled(false);
    startButton.setLookAndFeel(&buttonLookAndFeel);
    startButton.onClick = [this]()
    {
        this->startStop(true);
    };

    addAndMakeVisible(resetButton);
    resetButton.setLookAndFeel(&buttonLookAndFeel);
    resetButton.onClick = [this]()
    {
        this->reset();
        // audio gen first
    };

    addAndMakeVisible(stopButton);
    stopButton.setEnabled(false);
    stopButton.setLookAndFeel(&buttonLookAndFeel);
    stopButton.onClick = [this]()
    {
        this->startStop(false);
        // audio gen first
    };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 600);
}

MusicAEAudioProcessorEditor::~MusicAEAudioProcessorEditor()
{
}

//==============================================================================
void MusicAEAudioProcessorEditor::paint (juce::Graphics& g)
{
    // fill the whole window white
    g.fillAll (juce::Colour(0xff171752));
 
    // set the current drawing colour to black
    g.setColour (juce::Colours::white);
 
    // set the font size and draw text to the screen
    g.setFont (juce::Font(0.0625*getHeight(), juce::Font::bold));
 
    g.drawFittedText ("MusicAE", 0, 0.5*0.0625*getHeight(), 7*0.0625*getWidth(), 0.0625*getHeight(), juce::Justification::centred, 1);
}

void MusicAEAudioProcessorEditor::resized()
{
    //std::cerr << "resizing\n";
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // sets the position and size of the slider with arguments (x, y, width, height)
    float h = getHeight(), w = getWidth();
    alpha.setBounds (0.0625*w, 10.5*0.0625*h, 7*0.125*w, 0.0625*h);
    
    int i = 0;
    for(auto slider : latentSliders){
        slider->setBounds(0.5*0.0625*w + 1.5*0.0625*w*i, 0.125*h, 1.5*0.0625*w, 0.5*h);
        i++;
    }
    
    synthButton.setBounds(7*0.0625*w, 0.5*0.0625*h, 0.125*w, 0.0625*h);
    effectsButton.setBounds(10*0.0625*w, 0.5*0.0625*h, 0.125*w, 0.0625*h);
    mixerButton.setBounds(13*0.0625*w, 0.5*0.0625*h, 0.125*w, 0.0625*h);
    
    modelTextBox.setBounds(0.0625*w, 6*0.125*h, 0.5*w, 0.0625*h);
    modelTextBox.setFont(juce::Font(0.8*0.0625*h));
    loadButton.setBounds(5*0.125*w, 6*0.125*h, 5*0.0625*w, 0.0625*h);
    startButton.setBounds(0.0625*w, 14.5*0.0625*h, 4*0.0625*w, 0.0625*h);
    resetButton.setBounds(3*0.125*w, 14.5*0.0625*h, 4*0.0625*w, 0.0625*h);
    stopButton.setBounds(11*0.0625*w, 14.5*0.0625*h, 4*0.0625*w, 0.0625*h);
}

void MusicAEAudioProcessorEditor::startStop(bool start)
{
    if (!start){
        audioProcessor.process = false;
        audioProcessor.reset();
        reset();
    }
        
    switch(audioProcessor.state){
        case STATE_SYNTH:
            effectsButton.setVisible(!start);
            mixerButton.setVisible(!start);
            break;
        case STATE_EFFECTS:
            synthButton.setVisible(!start);
            mixerButton.setVisible(!start);
            break;
        case STATE_MIXER:
            synthButton.setVisible(!start);
            effectsButton.setVisible(!start);
            break;
    }
    
    loadButton.setEnabled(!start);
    startButton.setEnabled(!start);
    stopButton.setEnabled(start);
    audioProcessor.process = start;
}

void MusicAEAudioProcessorEditor::reset()
{
    alpha.setValue(0.5);
    
    for(auto slider : latentSliders)
        slider->setValue(50.0);
}

void MusicAEAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    //std::cerr << "slider value changed\n";
    if (audioProcessor.genInit){
        audioProcessor.generator->alpha = (float)alpha.getValue();
        for (int i = 0; i < latentSliders.size(); i++)
            audioProcessor.generator->sliders[i] = (float)latentSliders[i]->getValue() / 100;
    }
    else{
        audioProcessor.temp_alpha = (float)alpha.getValue();
        for (int i = 0; i < latentSliders.size(); i++)
            audioProcessor.temp_sliders[i] = (float)latentSliders[i]->getValue() / 100;
    }
}

void MusicAEAudioProcessorEditor::onStateChange(enum MusicAE_state new_state)
{
    std::cerr << new_state;
    if (new_state == audioProcessor.state)
        return;

    switch(new_state){
        case STATE_SYNTH:
            synthButton.setEnabled(false);
            effectsButton.setEnabled(true);
            mixerButton.setEnabled(true);
            break;
        case STATE_EFFECTS:
            synthButton.setEnabled(true);
            effectsButton.setEnabled(false);
            mixerButton.setEnabled(true);
            break;
        case STATE_MIXER:
            synthButton.setEnabled(true);
            effectsButton.setEnabled(true);
            mixerButton.setEnabled(false);
            break;
    }

        
    if (new_state != STATE_MIXER)
        alpha.setVisible(false);
    else
        alpha.setVisible(true);

    audioProcessor.updateState(new_state);
    
    reset();
}