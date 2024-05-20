/*
  ==============================================================================

    StemTypeCombo.h
    Created: 14 May 2024 9:09:27pm
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynergyLookAndFeel.h"

//==============================================================================
/*
*/
class StemTypeCombo  : public juce::Component
{
public:
    StemTypeCombo();
    ~StemTypeCombo() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // allows the stem type to only be clickable in a specified area
    bool hitTest(int x, int y) override;

    juce::ComboBox stemTypeCombo;

private:
    SynergyLookAndFeel synergyLookAndFeel;
    juce::Rectangle<int> hitArea{ 0, 0, 180, 100 }; // hit box area

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StemTypeCombo)
};
