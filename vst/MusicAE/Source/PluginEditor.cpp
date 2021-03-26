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
    //std::cerr << "plugin editor constructor\n";    
    // these define the parameters of our slider object
    alpha.setSliderStyle (juce::Slider::LinearHorizontal);
    alpha.setRange (0.0, 1.0, 0.01);
    alpha.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    alpha.setPopupDisplayEnabled (true, false, this);
    alpha.setTextValueSuffix (" alpha");
    alpha.setValue(0.5);
    alpha.addListener(this);
 
    // this function adds the slider to the editor
    addAndMakeVisible (&alpha);
        
    for(int i = 0; i < 10; i++){
        auto slider = latentSliders.add(new juce::Slider);
        slider->setSliderStyle (juce::Slider::LinearVertical);
        slider->setRange (0.0, 100.0, 0.01);
        slider->setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
        slider->setPopupDisplayEnabled (true, false, this);
        slider->setTextValueSuffix (" latent " + std::to_string(i));
        slider->setValue(50.0);
        slider->addListener(this);
        
        addAndMakeVisible(slider);
    }
    
    addAndMakeVisible(synthButton);
    synthButton.onClick = [this]()
    {
        this->onStateChange(STATE_SYNTH);
    };
    
    addAndMakeVisible(effectsButton);
    effectsButton.onClick = [this]()
    {
        this->onStateChange(STATE_EFFECTS);
    };
    
    addAndMakeVisible(mixerButton);
    mixerButton.onClick = [this]()
    {
        this->onStateChange(STATE_MIXER);
    };
    
    synthButton.setRadioGroupId(StateButtons);
    effectsButton.setRadioGroupId(StateButtons);
    mixerButton.setRadioGroupId(StateButtons);
    
    addAndMakeVisible(modelTextBox);

    loadButton.setButtonText("Load");
    addAndMakeVisible(loadButton);
    loadButton.onClick = [this]()
    {
        this->audioProcessor.generator->model_name = this->modelTextBox.getText().toStdString();
        this->audioProcessor.generator->modelToMem();
        this->startButton.setEnabled(true);
    };

    startButton.setButtonText("Start");
    addAndMakeVisible(startButton);
    startButton.setEnabled(false);
    startButton.onClick = [this]()
    {
        this->resetButton.setEnabled(true);
        this->audioProcessor.process = true;
    };

    resetButton.setButtonText("Reset");
    addAndMakeVisible(resetButton);
    resetButton.setEnabled(false);
    resetButton.onClick = [this]()
    {
        this->reset();
    };

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
    //std::cerr << "resizing\n";
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // sets the position and size of the slider with arguments (x, y, width, height)
    alpha.setBounds (50, 550, 700, 75);
    
    int i = 0;
    for(auto slider : latentSliders){
        slider->setBounds(30 + 80*i, 100, 50, 400);
        i++;
    }
    
    synthButton.setBounds(200, 650, 100, 50);
    effectsButton.setBounds(350, 650, 100, 50);
    mixerButton.setBounds(500, 650, 100, 50);
    
    modelTextBox.setBounds(100, 725, 200, 50);
    loadButton.setBounds(350, 725, 100, 50);
    startButton.setBounds(500, 725, 100, 50);
    resetButton.setBounds(650, 725, 100, 50);
}

void MusicAEAudioProcessorEditor::reset()
{
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
    if (new_state == audioProcessor.state)
        return;

    if (new_state == STATE_SYNTH)
        alpha.setVisible(false);
    else
        alpha.setVisible(true);

    audioProcessor.updateState(new_state);
    
    reset();
    
    startButton.setEnabled(false);
    resetButton.setEnabled(false);
    loadButton.setEnabled(true);
}