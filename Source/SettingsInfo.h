/*
  ==============================================================================

    SettingsInfo.h
    Created: 18 May 2024 1:05:02am
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynergyLookAndFeel.h"

using namespace juce;

//==============================================================================
/*
*/
class SettingsInfo  : public juce::Component
{
public:
    SettingsInfo();
    ~SettingsInfo() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SynergyLookAndFeel synergyLookAndFeel;

    // active buttons
    bool generalActive;
    bool audioActive;

    // general settings
    TextButton generalButton;
    Label generalTitle;
    Label licenseKeyLabel;
    Label buildVersionLabel;
    HyperlinkButton reportABugButton;

    // audio settings
    TextButton audioButton;
    Label audioTitle;

    Colour buttonBackgroundColor = Colour(20, 20, 20);
    Colour buttonHoverColor = Colour(25, 25, 25);

    void generalButtonClicked();
    void audioButtonClicked();

    void hideGeneralSettings();
    void showGeneralSettings();
    void hideAudioSettings();
    void showAudioSettings();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsInfo)
};
