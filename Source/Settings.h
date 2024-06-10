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
#include "SettingsCache.h"
#include "PluginProcessor.h"

using namespace juce;

//==============================================================================
/*
*/
class Settings  : public juce::DocumentWindow
{
public:
    Settings(SettingsCache& sc, SynergyAudioProcessor& p);
    ~Settings() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void closeButtonPressed() override
    {
        delete this;
    }

private:
    SettingsInfo* settingsInfo;
    SettingsCache* settingsCache;
    SynergyAudioProcessor* audioProcessor;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Settings)
};
