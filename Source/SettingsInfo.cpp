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
SettingsInfo::SettingsInfo(SettingsCache* sc, SynergyAudioProcessor* p) : settingsCache(sc), audioProcessor(p)
{
    setLookAndFeel(&synergyLookAndFeel);
    /*
    * General Settings
    */

    // license key
    // cached license key
    File licenseFile = File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile).getParentDirectory().getChildFile("sbr_license.txt");
    String licenseKey = "License Key: ";
    // if the license file exists we attempt to verify
    if (licenseFile.existsAsFile())
    {
        std::unique_ptr<FileInputStream> inputLicenseFile(licenseFile.createInputStream());

        // if the file was opened
        if (inputLicenseFile->openedOk())
        {
            licenseKey += inputLicenseFile->readString();
        }
    }

    licenseKeyLabel.setText(licenseKey, NotificationType::dontSendNotification);
    licenseKeyLabel.setColour(Label::textColourId, Colour(60, 60, 60));
    licenseKeyLabel.setFont(15.0f);
    addAndMakeVisible(licenseKeyLabel);

    // build version
    string versionString = "Build: v";
    buildVersionLabel.setText(versionString.append(ProjectInfo::versionString), NotificationType::dontSendNotification);
    buildVersionLabel.setColour(Label::textColourId, Colour(60, 60, 60));
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
    reportABugButton.setURL((URL)"https://www.rumoraudio.com/pages/contact");
    addAndMakeVisible(reportABugButton);

    // bassline loop setting
    basslineLoopSettingLabel.setText("Bassline Loop: ", NotificationType::dontSendNotification);
    basslineLoopSettingLabel.setFont(16.0f);
    addAndMakeVisible(basslineLoopSettingLabel);

    basslineLoopSettingCombo.addItem("4 Bars", 1);
    basslineLoopSettingCombo.addItem("8 Bars", 2);
    basslineLoopSettingCombo.setSelectedId(settingsCache->basslineLoop);
    basslineLoopSettingCombo.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    basslineLoopSettingCombo.setComponentID("BasslineLoop");
    basslineLoopSettingCombo.setColour(juce::ComboBox::backgroundColourId, Colour(15, 15, 15));
    basslineLoopSettingCombo.setColour(juce::ComboBox::textColourId, Colour(100, 100, 100));
    basslineLoopSettingCombo.setColour(juce::ComboBox::outlineColourId, Colour(200, 200, 200));
    basslineLoopSettingCombo.onChange = [this] { updateBasslineLoop(); };
    addAndMakeVisible(basslineLoopSettingCombo);

    // default variety
    defaultVarietyLabel.setText("Default Variety: ", NotificationType::dontSendNotification);
    defaultVarietyLabel.setFont(16.0f);
    addAndMakeVisible(defaultVarietyLabel);

    defaultVarietyTextEditor.setJustification(juce::Justification::centredTop);
    defaultVarietyTextEditor.setColour(TextEditor::backgroundColourId, Colour(15, 15, 15));
    defaultVarietyTextEditor.setColour(TextEditor::textColourId, Colour(100, 100, 100));
    defaultVarietyTextEditor.setColour(TextEditor::outlineColourId, Colour(40, 40, 40));
    defaultVarietyTextEditor.setText((String)settingsCache->defaultVariety);
    defaultVarietyTextEditor.setFont(12.0f);
    defaultVarietyTextEditor.onTextChange = [this] { updateDefaultVariety(); };
    defaultVarietyTextEditor.addListener(&textFilter);
    addAndMakeVisible(defaultVarietyTextEditor);

    // default velocity
    defaultVelocityLabel.setText("Default Density: ", NotificationType::dontSendNotification);
    defaultVelocityLabel.setFont(16.0f);
    addAndMakeVisible(defaultVelocityLabel);

    defaultVelocityTextEditor.setJustification(juce::Justification::centredTop);
    defaultVelocityTextEditor.setColour(TextEditor::backgroundColourId, Colour(15, 15, 15));
    defaultVelocityTextEditor.setColour(TextEditor::textColourId, Colour(100, 100, 100));
    defaultVelocityTextEditor.setColour(TextEditor::outlineColourId, Colour(40, 40, 40));
    defaultVelocityTextEditor.setText((String)settingsCache->defaultNoteVelocity);
    defaultVelocityTextEditor.setFont(12.0f);
    defaultVelocityTextEditor.onTextChange = [this] { updateDefaultVelocity(); };
    defaultVelocityTextEditor.addListener(&textFilter);
    addAndMakeVisible(defaultVelocityTextEditor);

    // tool tips
    toolTipsLabel.setText("Enable Tooltips: ", NotificationType::dontSendNotification);
    toolTipsLabel.setFont(16.0f);
    addAndMakeVisible(toolTipsLabel);

    toolTipsToggleButton.setToggleState(settingsCache->toolTips, juce::dontSendNotification); // Set default state
    toolTipsToggleButton.setColour(ToggleButton::tickColourId, Colour(100, 100, 100));
    toolTipsToggleButton.setMouseCursor(MouseCursor::PointingHandCursor);
    toolTipsToggleButton.onClick = [this] { updateTooltip(); };
    addAndMakeVisible(toolTipsToggleButton);

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

    // preview bass
    previewBassLabel.setText("Preview Bass: ", NotificationType::dontSendNotification);
    previewBassLabel.setFont(16.0f);
    addChildComponent(previewBassLabel);

    previewBassComboBox.addItem("Outta Space", 1);
    previewBassComboBox.addItem("Motion 808", 2);
    previewBassComboBox.addItem("Rock 808", 3);
    previewBassComboBox.addItem("Wow 808", 4);
    previewBassComboBox.addItem("Wub", 5);
    previewBassComboBox.setSelectedId(settingsCache->previewBass);
    previewBassComboBox.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    previewBassComboBox.setComponentID("PreviewBass");
    previewBassComboBox.setColour(juce::ComboBox::backgroundColourId, Colour(15, 15, 15));
    previewBassComboBox.setColour(juce::ComboBox::textColourId, Colour(100, 100, 100));
    previewBassComboBox.setColour(juce::ComboBox::outlineColourId, Colour(200, 200, 200));
    previewBassComboBox.onChange = [this] { updatePreviewBass(); };
    addChildComponent(previewBassComboBox);
}

SettingsInfo::~SettingsInfo()
{
    saveSettings();
    setLookAndFeel(nullptr);
    defaultVarietyTextEditor.removeListener(&textFilter);
    defaultVelocityTextEditor.removeListener(&textFilter);
    audioProcessor->setPreviewBass(settingsCache->previewBass);
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


    // bassline loop setting
    basslineLoopSettingLabel.setBounds(160, 40, 120, 20);
    basslineLoopSettingCombo.setBounds(280, 40, 60, 20);

    // default variety setting
    defaultVarietyLabel.setBounds(160, 70, 120, 20);
    defaultVarietyTextEditor.setBounds(280, 70, 60, 20);

    // default velocity setting
    defaultVelocityLabel.setBounds(160, 100, 120, 20);
    defaultVelocityTextEditor.setBounds(280, 100, 60, 20);

    // tool tips setting
    toolTipsLabel.setBounds(160, 130, 120, 20);
    toolTipsToggleButton.setBounds(276, 130, 21, 20);

    // build version label
    buildVersionLabel.setBounds(160, 170, 300, 20);
    // license key label
    licenseKeyLabel.setBounds(160, 190, 300, 20);
    // report a bug link
    reportABugButton.setBounds(410, 375, 90, 20);
    

    /*
    * Audio Settings
    */

    // audio title
    audioTitle.setBounds(160, 10, 300, 20);

    // preview bass setting
    previewBassLabel.setBounds(160, 40, 120, 20);
    previewBassComboBox.setBounds(270, 40, 120, 20);

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
    basslineLoopSettingLabel.setVisible(false);
    basslineLoopSettingCombo.setVisible(false);
    defaultVarietyLabel.setVisible(false);
    defaultVarietyTextEditor.setVisible(false);
    defaultVelocityLabel.setVisible(false);
    defaultVelocityTextEditor.setVisible(false);
    toolTipsLabel.setVisible(false);
    toolTipsToggleButton.setVisible(false);
}

void SettingsInfo::showGeneralSettings() {
    generalTitle.setVisible(true);
    licenseKeyLabel.setVisible(true);
    buildVersionLabel.setVisible(true);
    basslineLoopSettingLabel.setVisible(true);
    basslineLoopSettingCombo.setVisible(true);
    defaultVarietyLabel.setVisible(true);
    defaultVarietyTextEditor.setVisible(true);
    defaultVelocityLabel.setVisible(true);
    defaultVelocityTextEditor.setVisible(true);
    toolTipsLabel.setVisible(true);
    toolTipsToggleButton.setVisible(true);
}

void SettingsInfo::hideAudioSettings() {
    audioTitle.setVisible(false);
    previewBassLabel.setVisible(false);
    previewBassComboBox.setVisible(false);
}

void SettingsInfo::showAudioSettings() {
    audioTitle.setVisible(true);
    previewBassLabel.setVisible(true);
    previewBassComboBox.setVisible(true);
}

void SettingsInfo::saveSettings()
{
    // create JSON to store settings
    DynamicObject* jsonData = new DynamicObject();
    jsonData->setProperty("basslineLoop", basslineLoopSettingCombo.getSelectedId());
    jsonData->setProperty("defaultVariety", defaultVarietyTextEditor.getText());
    jsonData->setProperty("defaultNoteVelocity", defaultVelocityTextEditor.getText());
    jsonData->setProperty("previewBass", previewBassComboBox.getSelectedId());
    jsonData->setProperty("toolTips", toolTipsToggleButton.getToggleStateValue().getValue());

    // Serialize the JSON object into a string
    var jsonVar(jsonData);
    String jsonString = JSON::toString(jsonVar, true);

    File settingsFile(File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile).getParentDirectory().getChildFile("sbr_settings.txt"));
    settingsFile.replaceWithText(jsonString);
}

void SettingsInfo::updateBasslineLoop()
{
    settingsCache->basslineLoop = basslineLoopSettingCombo.getSelectedId();
}

void SettingsInfo::updateDefaultVariety()
{
    String text = defaultVarietyTextEditor.getText();
    settingsCache->defaultVariety = text.getIntValue();
}

void SettingsInfo::updateDefaultVelocity()
{
    String text = defaultVelocityTextEditor.getText();
    settingsCache->defaultNoteVelocity = text.getIntValue();
}

void SettingsInfo::updatePreviewBass()
{
    settingsCache->previewBass = previewBassComboBox.getSelectedId();
}

void SettingsInfo::updateTooltip()
{
    settingsCache->toolTips = toolTipsToggleButton.getToggleStateValue().getValue();
}
