/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SmartNoiseGateAudioProcessor::SmartNoiseGateAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // Add parameters
    addParameter(thresholdParam = new juce::AudioParameterFloat(
        "threshold",           // parameter ID
        "Threshold",           // parameter name
        0.0001f,              // min value
        0.1f,                 // max value
        0.001f));             // default value
        
    addParameter(attackParam = new juce::AudioParameterFloat(
        "attack",             // parameter ID
        "Attack",             // parameter name
        0.001f,              // min value (1ms)
        0.1f,                // max value (100ms)
        0.01f));             // default value (10ms)
        
    addParameter(releaseParam = new juce::AudioParameterFloat(
        "release",            // parameter ID
        "Release",            // parameter name
        0.01f,               // min value (10ms)
        1.0f,                // max value (1000ms)
        0.1f));              // default value (100ms)
}

SmartNoiseGateAudioProcessor::~SmartNoiseGateAudioProcessor()
{
}

//==============================================================================
const juce::String SmartNoiseGateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SmartNoiseGateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SmartNoiseGateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SmartNoiseGateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SmartNoiseGateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SmartNoiseGateAudioProcessor::getNumPrograms()
{
    return 1;   // Keeping this at 1 because some hosts can't handle basic math
                
}

int SmartNoiseGateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SmartNoiseGateAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SmartNoiseGateAudioProcessor::getProgramName (int index)
{
    return {};
}

void SmartNoiseGateAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SmartNoiseGateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Reset gate envelope when playback starts
    gateEnvelope = 1.0f;
}

void SmartNoiseGateAudioProcessor::releaseResources()
{
    // Playback finished. Free up any leftover resources here.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SmartNoiseGateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    //  Only support mono or stereo.
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

void SmartNoiseGateAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // ===== GATE LOGIC STARTS HERE =====
    
    // Calculate RMS energy of the frame
    float energy = 0.0f;
    int totalSamples = buffer.getNumSamples() * totalNumInputChannels;
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getReadPointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float sampleValue = channelData[sample];
            energy += sampleValue * sampleValue;
        }
    }
    
    // Average energy across all samples
    if (totalSamples > 0)
        energy = energy / totalSamples;
    
    // Get parameter values
    float threshold = thresholdParam->get();
    float attack = attackParam->get();
    float release = releaseParam->get();
    
    // Determine if gate should be open or closed
    float targetGain = (energy > threshold) ? 1.0f : 0.0f;
    
    // Smooth the gate envelope with attack/release
    float coeff = (targetGain > gateEnvelope) ? attack : release;
    gateEnvelope += (targetGain - gateEnvelope) * coeff;
    
    // Apply gate envelope to all channels
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= gateEnvelope;
        }
    }
    
    // ===== GATE LOGIC ENDS HERE =====
}

//==============================================================================
bool SmartNoiseGateAudioProcessor::hasEditor() const
{
    return true; // 
}

juce::AudioProcessorEditor* SmartNoiseGateAudioProcessor::createEditor()
{
    return new SmartNoiseGateAudioProcessorEditor (*this);
}

//==============================================================================
void SmartNoiseGateAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save the state here. We can just dump raw data, but using a ValueTree or XML is way easier for handling complex parameters.
}

void SmartNoiseGateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Use this method to restore parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SmartNoiseGateAudioProcessor();
}
