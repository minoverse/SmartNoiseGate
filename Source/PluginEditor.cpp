/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SmartNoiseGateAudioProcessorEditor::SmartNoiseGateAudioProcessorEditor (SmartNoiseGateAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    // editor's size 
    setSize (400, 300);
}

SmartNoiseGateAudioProcessorEditor::~SmartNoiseGateAudioProcessorEditor()
{
}

//==============================================================================
void SmartNoiseGateAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill the background with a solid colour
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SmartNoiseGateAudioProcessorEditor::resized()
{
    
}
