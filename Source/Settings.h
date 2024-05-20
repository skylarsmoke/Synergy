/*
  ==============================================================================

    Settings.h
    Created: 18 May 2024 12:39:33am
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SettingsInfo.h"

using namespace juce;

//==============================================================================
/*
*/
class Settings  : public juce::DocumentWindow
{
public:
    Settings();
    ~Settings() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void closeButtonPressed() override
    {
        delete this;
    }

private:
    SettingsInfo* settingsInfo;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Settings)
};
