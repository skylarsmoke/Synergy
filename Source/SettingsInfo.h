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
#include "SettingsCache.h"
#include "DefaultSliderFilter.h"
#include "PluginProcessor.h"

using namespace juce;

//==============================================================================
/*
*/
class SettingsInfo  : public juce::Component
{
public:
    SettingsInfo(SettingsCache* sc, SynergyAudioProcessor* p);
    ~SettingsInfo() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SynergyLookAndFeel synergyLookAndFeel;
    SettingsCache* settingsCache;
    SynergyAudioProcessor* audioProcessor;
    DefaultSliderFilter textFilter;

    // active buttons
    bool generalActive;
    bool audioActive;

    // general settings
    TextButton generalButton;
    Label generalTitle;
    Label licenseKeyLabel;
    Label buildVersionLabel;
    HyperlinkButton reportABugButton;

    Label basslineLoopSettingLabel;
    ComboBox basslineLoopSettingCombo;

    Label defaultVarietyLabel;
    TextEditor defaultVarietyTextEditor;

    Label defaultVelocityLabel;
    TextEditor defaultVelocityTextEditor;

    // audio settings
    TextButton audioButton;
    Label audioTitle;

    Label previewBassLabel;
    ComboBox previewBassComboBox;

    Colour buttonBackgroundColor = Colour(20, 20, 20);
    Colour buttonHoverColor = Colour(25, 25, 25);

    void generalButtonClicked();
    void audioButtonClicked();

    void hideGeneralSettings();
    void showGeneralSettings();
    void hideAudioSettings();
    void showAudioSettings();

    /*
    * Settings Maintenance
    */

    /// <summary>
    /// Saves current settings
    /// </summary>
    void saveSettings();
    void updateBasslineLoop();
    void updateDefaultVariety();
    void updateDefaultVelocity();
    void updatePreviewBass();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsInfo)
};
