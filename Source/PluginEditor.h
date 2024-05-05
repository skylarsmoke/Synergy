/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Theme.h"

//==============================================================================
/**
*/
class SynergyAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener
{
public:
    SynergyAudioProcessorEditor (SynergyAudioProcessor&);
    ~SynergyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    void sliderValueChanged(juce::Slider* slider) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynergyAudioProcessor& audioProcessor;

    Theme* theme;

    juce::Slider midiVolume;
    bool developmentMode = true;
    bool blackAndWhiteTheme = true;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynergyAudioProcessorEditor)
};
