/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MusicAEAudioProcessor::MusicAEAudioProcessor()
    :
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    temp_sliders(10, 0.5)
{
    generator = new AudioGenerator(batches, sampRate, chunk, temp_sliders);
    updateState(STATE_SYNTH);
    genInit = true;
}

MusicAEAudioProcessor::~MusicAEAudioProcessor()
{
}

//==============================================================================
const juce::String MusicAEAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MusicAEAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MusicAEAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MusicAEAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MusicAEAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MusicAEAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MusicAEAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MusicAEAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MusicAEAudioProcessor::getProgramName (int index)
{
    return {};
}

void MusicAEAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MusicAEAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    reset();

    sampRate = sampleRate;
}

void MusicAEAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MusicAEAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MusicAEAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    fltFadeBuffer = processSamples(buffer, midiMessages, fltDelayBuffer, fltFadeBuffer);
}

void MusicAEAudioProcessor::processBlock (juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    dblFadeBuffer = processSamples(buffer, midiMessages, dblDelayBuffer, dblFadeBuffer);
}

template <typename Real>
juce::AudioBuffer<Real> MusicAEAudioProcessor::processSamples (juce::AudioBuffer<Real>& buffer, juce::MidiBuffer& midiMessages, juce::AudioBuffer<Real>& delayBuffer, juce::AudioBuffer<Real>& fadeBuffer)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    const int bufferLength = buffer.getNumSamples();
    const int delayBufferLength = delayBuffer.getNumSamples();
        
    if (process) {
        int numChannels = state == STATE_MIXER ? 2 : state == STATE_EFFECTS ? 1 : 0;
        for (int channel = 0; channel < numChannels; ++channel)
        {
            if (delayBufferWriteIndex + bufferLength < delayBufferLength)
                delayBuffer.copyFrom(channel, delayBufferWriteIndex, buffer, channel, 0, bufferLength);
            else{
                const int spaceRemaining = delayBufferLength - delayBufferWriteIndex;
                delayBuffer.copyFrom(channel, delayBufferWriteIndex, buffer, channel, 0, spaceRemaining);
                delayBuffer.copyFrom(channel, 0, buffer, channel, spaceRemaining, bufferLength - spaceRemaining);
            }
        }
          
        delayBufferWriteIndex += bufferLength;
        delayBufferWriteIndex %= delayBufferLength;
        delayBufferProcessCounter += bufferLength;
    
        while (delayBufferProcessCounter >= batches * chunk){
            int processStart = (delayBufferWriteIndex - delayBufferProcessCounter) % delayBufferLength;
            const Real *excessPtr1 = nullptr;
            const Real *excessPtr2 = nullptr;
            bool wrap = false;
            int excess = 0;
            if (processStart < 0) {
                excessPtr1 = delayBuffer.getReadPointer(0, 0);
                if (state == STATE_MIXER)
                    excessPtr2 = delayBuffer.getReadPointer(1, 0);
                    
                processStart += delayBufferLength;
                wrap = processStart > delayBufferLength - (batches - overlap) * chunk;
                excess = delayBufferWriteIndex;
            }
            
            // run model replace values
            const Real *secondPtr = state == STATE_MIXER ? delayBuffer.getReadPointer(1, processStart) : nullptr;
            Real *newData = generator->genAudio<Real>(delayBuffer.getReadPointer(0, processStart), excessPtr1, secondPtr, excessPtr2, excess, fadeBuffer.getReadPointer(0, 0), state);
            if (state != STATE_SYNTH){
                fadeBuffer.copyFrom(0, 0, newData + (batches - overlap) * chunk, chunk);
            }
            
            if (wrap) {
                delayBuffer.copyFrom(0, processStart, newData, delayBufferLength - processStart);
                delayBuffer.copyFrom(0, 0, newData + (batches * chunk - delayBufferWriteIndex), (batches - overlap) * chunk - (delayBufferLength - processStart));
            }
            else 
                delayBuffer.copyFrom(0, processStart, newData, (batches - overlap) * chunk);
            
            delete newData;
            delayBufferProcessCounter -= (batches - overlap) * chunk;
        }
        delayBufferReadIndex += bufferLength;
        delayBufferReadIndex %= delayBufferLength;
        
        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            if (delayBufferReadIndex + bufferLength < delayBufferLength)
                buffer.copyFrom(channel, 0, delayBuffer, 0, delayBufferReadIndex, bufferLength);
            else{
                const int spaceRemaining = delayBufferLength - delayBufferReadIndex;
                buffer.copyFrom(channel, 0, delayBuffer, 0, delayBufferReadIndex, spaceRemaining);
                buffer.copyFrom(channel, spaceRemaining, delayBuffer, 0, 0, bufferLength - spaceRemaining);
            }
        }
    }
    
    return fadeBuffer;
}

//==============================================================================
bool MusicAEAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MusicAEAudioProcessor::createEditor()
{
    return new MusicAEAudioProcessorEditor (*this);
}

//==============================================================================
void MusicAEAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MusicAEAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void MusicAEAudioProcessor::reset()
{
    int numChannels = state == STATE_MIXER ? 2 : 1;
    if (isUsingDoublePrecision()){
        dblDelayBuffer.setSize(numChannels, (2 * batches - overlap) * chunk);
        dblFadeBuffer.setSize(1, chunk);
        delayBufferReadIndex = dblDelayBuffer.getNumSamples() - batches * chunk;
        dblDelayBuffer.clear();
    }
    else{
        fltDelayBuffer.setSize(numChannels, (2 * batches - overlap) * chunk);
        fltFadeBuffer.setSize(1, chunk);
        delayBufferReadIndex = fltDelayBuffer.getNumSamples() - batches * chunk;
        fltDelayBuffer.clear();
    }
    
    delayBufferWriteIndex = 0;
    delayBufferProcessCounter = 0;
}

void MusicAEAudioProcessor::updateState(enum MusicAE_state new_state)
{
    state = new_state;
    batches = state == STATE_SYNTH ? 20 : 24;
    overlap = state == STATE_SYNTH ? 0 : 4;
    
    reset();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MusicAEAudioProcessor();
}