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
    //std::cerr << "processor constructor\n";
    //createEditor();
    //suspendProcessing(true);
    generator = new AudioGenerator(batches, sampRate, chunk, temp_sliders);
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
    //std::cerr << "prepare to play ";

    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    if (isUsingDoublePrecision()){
        //std::cerr << "double\n";
        dblDelayBuffer.setSize(1, 2 * batches * chunk);
        delayBufferReadIndex = dblDelayBuffer.getNumSamples() - batches * chunk;
        dblDelayBuffer.clear();
    }
    else{
        //std::cerr << "float\n";
        fltDelayBuffer.setSize(1, 2 * batches * chunk);
        delayBufferReadIndex = fltDelayBuffer.getNumSamples() - batches * chunk;
        fltDelayBuffer.clear();
    }

    sampRate = sampleRate;
    
    /*std::cerr << "sampling rate: " << sampleRate << "\n";
    std::cerr << "samples per block estimate: " << samplesPerBlock << "\n";*/
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
    //std::cerr << "process block float\n";
    processSamples(buffer, midiMessages, fltDelayBuffer);
}

void MusicAEAudioProcessor::processBlock (juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    //std::cerr << "process block double\n";
    processSamples(buffer, midiMessages, dblDelayBuffer);
}

template <typename Real>
void MusicAEAudioProcessor::processSamples (juce::AudioBuffer<Real>& buffer, juce::MidiBuffer& midiMessages, juce::AudioBuffer<Real>& delayBuffer)
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
    
    //std::cerr << "num samples: " << bufferLength << "\n";

    if (process) {
        //std::cerr << "processing\n";
        /*for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            // TODO: buffer larger than delay buffer?
            if (delayBufferWriteIndex + bufferLength < delayBufferLength)
                delayBuffer.copyFrom(0, delayBufferWriteIndex, buffer, channel, 0, bufferLength);
            else{
                const int spaceRemaining = delayBufferLength - delayBufferWriteIndex;
                delayBuffer.copyFrom(0, delayBufferWriteIndex, buffer, channel, 0, spaceRemaining);
                delayBuffer.copyFrom(0, 0, buffer, channel, spaceRemaining, bufferLength - spaceRemaining);
            }*/

            
            
            delayBufferWriteIndex += bufferLength;
            delayBufferWriteIndex %= delayBufferLength;
            delayBufferProcessCounter += bufferLength;

            /*std::cerr << "delay buffer write index after update: " << delayBufferWriteIndex << "\n";
            std::cerr << "delay buffer process counter after update: " << delayBufferProcessCounter << "\n";*/
            
            while (delayBufferProcessCounter >= batches * chunk){
                std::cerr << "running genaudio\n";
                
                int processStart = (delayBufferWriteIndex - delayBufferProcessCounter) % delayBufferLength;
                if (processStart < 0)
                    processStart += delayBufferLength;
                std::cerr << "processStart: " << processStart << "\n";
                // run model replace values
                Real *newData = generator->genAudio<Real>(delayBuffer.getReadPointer(0, processStart));

                delayBuffer.copyFrom(0, processStart, newData, batches * chunk);
                delayBufferProcessCounter -= batches * chunk;
            }
        //}
        
        delayBufferReadIndex += bufferLength;
        delayBufferReadIndex %= delayBufferLength;
        std::cerr << "delay buffer read index before filling: " << delayBufferReadIndex << "\n";
        
        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            if (delayBufferReadIndex + bufferLength < delayBufferLength)
                buffer.copyFrom(channel, 0, delayBuffer, 0, delayBufferReadIndex, bufferLength);
            else{
                const int spaceRemaining = delayBufferLength - delayBufferReadIndex;
                buffer.copyFrom(channel, 0, delayBuffer, 0, delayBufferReadIndex, spaceRemaining);
                buffer.copyFrom(channel, spaceRemaining, buffer, 0, 0, bufferLength - spaceRemaining);
            }
        }

        if (process) {
            for (int i = 0; i < chunk; i++)
                std::cerr << *buffer.getReadPointer(0, i) << "\n";
        }
    }
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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MusicAEAudioProcessor();
}
