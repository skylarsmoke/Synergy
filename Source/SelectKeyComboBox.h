/*
  ==============================================================================

    SelectKeyComboBox.h
    Created: 15 May 2024 1:24:52pm
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynergyLookAndFeel.h"

//==============================================================================
/*
*/
class SelectKeyComboBox  : public juce::Component
{
public:
    SelectKeyComboBox();
    ~SelectKeyComboBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::ComboBox selectKeyCombo;

private:
    SynergyLookAndFeel synergyLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SelectKeyComboBox)
};
