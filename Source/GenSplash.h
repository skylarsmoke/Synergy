/*
  ==============================================================================

    GenSplash.h
    Created: 5 Jun 2024 9:31:30pm
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <random>

//==============================================================================
/*
*/
class GenSplash  : public juce::Component, private juce::Timer
{
public:
    GenSplash();
    ~GenSplash() override;

    void paint (juce::Graphics&) override;
    void resized() override;


private:

    void timerCallback() override;

    float phase;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenSplash)
};
