/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SmartNoiseGateAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SmartNoiseGateAudioProcessorEditor (SmartNoiseGateAudioProcessor&);
    ~SmartNoiseGateAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is a quick way  to  access the processor object that created it.
    
    SmartNoiseGateAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmartNoiseGateAudioProcessorEditor)
};
