/*
  ==============================================================================

    SettingsInfo.cpp
    Created: 18 May 2024 1:05:02am
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SettingsInfo.h"
#include <string>

using namespace std;

//==============================================================================
SettingsInfo::SettingsInfo()
{
    setLookAndFeel(&synergyLookAndFeel);

    /*
    * General Settings
    */

    // license key
    licenseKeyLabel.setText("License Key:", NotificationType::dontSendNotification);
    licenseKeyLabel.setFont(15.0f);
    addAndMakeVisible(licenseKeyLabel);

    // build version
    string versionString = "Build: v";
    buildVersionLabel.setText(versionString.append(ProjectInfo::versionString), NotificationType::dontSendNotification);
    buildVersionLabel.setFont(15.0f);
    addAndMakeVisible(buildVersionLabel);

    // general settings title
    generalTitle.setText("General Settings", NotificationType::dontSendNotification);
    generalTitle.setFont(20.0f);
    generalTitle.setColour(Label::textColourId, Colour(120, 120, 120));
    addAndMakeVisible(generalTitle);

    // general settings button
    generalButton.setButtonText("General");
    generalButton.setColour(TextButton::buttonColourId, buttonBackgroundColor);
    generalButton.setColour(TextButton::buttonOnColourId, buttonBackgroundColor);
    generalButton.setMouseCursor(MouseCursor::PointingHandCursor);
    generalButton.onClick = [this] { generalButtonClicked(); };
    generalButtonClicked();
    addAndMakeVisible(generalButton);

    // report a bug link
    reportABugButton.setButtonText("Report a bug.");
    reportABugButton.setColour(HyperlinkButton::textColourId, Colour(60, 60, 60));
    reportABugButton.setURL((URL)"https://www.google.com");
    addAndMakeVisible(reportABugButton);

    /*
    * Audio Settings
    */

    // audio settings button
    audioButton.setButtonText("Audio");
    audioButton.setColour(TextButton::buttonColourId, buttonBackgroundColor);
    audioButton.setColour(TextButton::buttonOnColourId, buttonBackgroundColor);
    audioButton.setMouseCursor(MouseCursor::PointingHandCursor);
    audioButton.onClick = [this] { audioButtonClicked(); };
    addAndMakeVisible(audioButton);

    audioTitle.setText("Audio Settings", NotificationType::dontSendNotification);
    audioTitle.setFont(20.0f);
    audioTitle.setColour(Label::textColourId, Colour(120, 120, 120));
    audioTitle.setVisible(false);
    addChildComponent(audioTitle);
}

SettingsInfo::~SettingsInfo()
{
    setLookAndFeel(nullptr);
}

void SettingsInfo::paint (juce::Graphics& g)
{
    
    g.fillAll (Colour(25,25,25));  
    g.setColour(Colour(20, 20, 20));
    g.fillRect(0, 0, 150, 400);
}

void SettingsInfo::resized()
{
    /*
    * General Settings
    */

    // general title
    generalTitle.setBounds(160, 10, 300, 20);
    // build version label
    buildVersionLabel.setBounds(160, 40, 300, 20);
    // license key label
    licenseKeyLabel.setBounds(160, 60, 300, 20);
    // report a bug link
    reportABugButton.setBounds(410, 375, 90, 20);

    /*
    * Audio Settings
    */

    // audio title
    audioTitle.setBounds(160, 10, 300, 20);

    // list view buttons
    generalButton.setBounds(0, 0, 150, 40);
    audioButton.setBounds(0, 40, 150, 40);
}

void SettingsInfo::generalButtonClicked() {
    // deactivate other buttons
    audioActive = false;
    audioButton.setColour(TextButton::buttonColourId, buttonBackgroundColor);
    audioButton.setColour(TextButton::buttonOnColourId, buttonBackgroundColor);
    hideAudioSettings();

    // activate general button
    generalActive = true;
    generalButton.setColour(TextButton::buttonColourId, buttonHoverColor);
    generalButton.setColour(TextButton::buttonOnColourId, buttonHoverColor);
    showGeneralSettings();
}

void SettingsInfo::audioButtonClicked() {
    // deactivate other buttons
    generalActive = false;
    generalButton.setColour(TextButton::buttonColourId, buttonBackgroundColor);
    generalButton.setColour(TextButton::buttonOnColourId, buttonBackgroundColor);
    hideGeneralSettings();

    // activate audio button
    audioActive = true;
    audioButton.setColour(TextButton::buttonColourId, buttonHoverColor);
    audioButton.setColour(TextButton::buttonOnColourId, buttonHoverColor);
    showAudioSettings();
}

void SettingsInfo::hideGeneralSettings() {
    generalTitle.setVisible(false);
    licenseKeyLabel.setVisible(false);
    buildVersionLabel.setVisible(false);
}

void SettingsInfo::showGeneralSettings() {
    generalTitle.setVisible(true);
    licenseKeyLabel.setVisible(true);
    buildVersionLabel.setVisible(true);
}

void SettingsInfo::hideAudioSettings() {
    audioTitle.setVisible(false);
}

void SettingsInfo::showAudioSettings() {
    audioTitle.setVisible(true);
}
