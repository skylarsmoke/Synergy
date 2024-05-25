/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <ctime>
#include <string>

using namespace std;
using namespace juce;

void SynergyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider->getComponentID() == "NoteVelocity")
    {
        noteVelocityValue.setText((juce::String)noteVelocitySlider.getValue(), juce::NotificationType::dontSendNotification);
    }
    else if (slider->getComponentID() == "Variety") {
        varietySliderValue.setText((juce::String)varietySlider.getValue(), juce::NotificationType::dontSendNotification);
    }
    
}

//==============================================================================
SynergyAudioProcessorEditor::SynergyAudioProcessorEditor (SynergyAudioProcessor& p)
    : AudioProcessorEditor (&p), midiFileDrop (p, messageBox), audioProcessor (p), startTime (Time::getMillisecondCounterHiRes() * 0.001),
    productLockScreen(&productUnlockStatus, messageBox)
{

    startTimer(100);
    
    setSize (900, 700);
    synergyFont.setBold(true);
    synergyFont.setHeight(14.0f);
    //synergyFont.setTypefaceName("Stars Fighters");

    // theme object
    theme = new Theme();
    
    setLookAndFeel(&synergyLookAndFeel);

    // note velocity slider
    noteVelocitySlider.setSliderStyle(Slider::SliderStyle::LinearBar);
    noteVelocitySlider.setRange(0, 100, 1);
    noteVelocitySlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    noteVelocitySlider.setColour(Slider::textBoxTextColourId, Colours::transparentBlack);
    noteVelocitySlider.setValue(100);
    noteVelocitySlider.setMouseCursor(MouseCursor::PointingHandCursor);
    noteVelocitySlider.setTextBoxIsEditable(false);
    noteVelocitySlider.setComponentID("NoteVelocity");
    noteVelocitySlider.addListener(this);
    addAndMakeVisible(noteVelocitySlider);
    addAndMakeVisible(velocitySliderOverlay);
    
    noteVelocityValue.setText((juce::String)noteVelocitySlider.getValue(), juce::NotificationType::dontSendNotification);
    noteVelocityValue.setColour(juce::Label::textColourId, theme->mainSliderColor);
    noteVelocityValue.setJustificationType(Justification::centred);
    addAndMakeVisible(noteVelocityValue);

    // variety slider
    varietySlider.setSliderStyle(Slider::SliderStyle::LinearBar);
    varietySlider.setRange(0, 100, 1);
    varietySlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    varietySlider.setColour(Slider::textBoxTextColourId, Colours::transparentBlack);
    varietySlider.setValue(100);
    varietySlider.setMouseCursor(MouseCursor::PointingHandCursor);
    varietySlider.setTextBoxIsEditable(false);
    varietySlider.setComponentID("Variety");
    varietySlider.addListener(this);
    addAndMakeVisible(varietySlider);
    addAndMakeVisible(varietySliderOverlay);

    varietySliderValue.setText((juce::String)varietySlider.getValue(), juce::NotificationType::dontSendNotification);
    varietySliderValue.setColour(juce::Label::textColourId, theme->mainSliderColor);
    varietySliderValue.setJustificationType(Justification::centred);
    addAndMakeVisible(varietySliderValue);

    // generate button
    addAndMakeVisible(generateButton);

    // settings button
    Image settingsButtonImage = ImageCache::getFromMemory(BinaryData::settingsIcon_png, BinaryData::settingsIcon_pngSize);
    Image settingsButtonImageHover = ImageCache::getFromMemory(BinaryData::settingsIconHover_png, BinaryData::settingsIconHover_pngSize);
    settingsButton.setImages(false, true, true, settingsButtonImage, 1.0f, {}, settingsButtonImageHover, 1.0f, {}, settingsButtonImage, 1.0f, {});
    settingsButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    settingsButton.onClick = [this] { openSettings(); };
    addAndMakeVisible(settingsButton);

    // preview button
    Image previewButtonImage = ImageCache::getFromMemory(BinaryData::previewIcon_png, BinaryData::previewIcon_pngSize);
    Image previewButtonImageHover = ImageCache::getFromMemory(BinaryData::previewIconHover_png, BinaryData::previewIconHover_pngSize);
    previewButton.setImages(false, true, true, previewButtonImage, 1.0f, {}, previewButtonImageHover, 1.0f, {}, previewButtonImage, 1.0f, {});
    previewButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addAndMakeVisible(previewButton);

    // record button
    Image recordButtonImage = ImageCache::getFromMemory(BinaryData::recordIcon_png, BinaryData::recordIcon_pngSize);
    Image recordButtonImageHover = ImageCache::getFromMemory(BinaryData::recordIconHover_png, BinaryData::recordIconHover_pngSize);
    recordButton.setImages(false, true, true, recordButtonImage, 1.0f, {}, recordButtonImageHover, 1.0f, {}, recordButtonImage, 1.0f, {});
    recordButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addAndMakeVisible(recordButton);

    // dev mode label
    devModeLabel.setText("DEVELOPMENT MODE", NotificationType::dontSendNotification);
    devModeLabel.setColour(Label::textColourId, Colour(0,195,255));
    devModeLabel.setFont(synergyFont);
    if (developmentMode) addAndMakeVisible(devModeLabel);

    // stem type combo box
    addAndMakeVisible(stemTypeCombo);

    // select key combo box  
    addAndMakeVisible(selectKeyCombo);

    // defining parameters of our slider
    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Volume");
    midiVolume.setValue(1.0);
    midiVolume.setColour(1, juce::Colour(100,100,100));
    midiVolume.addListener(this);
    
    // add slider to editor
    //addAndMakeVisible(&midiVolume);

    // midi button
    bassDrumButton.setButtonText("Bass Drum (36)");
    bassDrumButton.onClick = [this] { setNoteNumber(60); };
    //addAndMakeVisible(bassDrumButton);

    // midi message box
    messageBox.setMultiLine(true);
    messageBox.setReturnKeyStartsNewLine(true);
    messageBox.setReadOnly(true);
    messageBox.setScrollbarsShown(true);
    messageBox.setCaretVisible(false);
    messageBox.setPopupMenuEnabled(true);
    messageBox.setColour(TextEditor::backgroundColourId, Colour(50, 50, 50));
    messageBox.setColour(TextEditor::outlineColourId, Colour(60, 60, 60));
    messageBox.setColour(TextEditor::shadowColourId, Colour(200, 200, 200));
    if (developmentMode) addAndMakeVisible(messageBox);

    // drag and drop midi
    addAndMakeVisible(midiFileDrop);
    
    /*
    * Everything after this needs to be last
    */

    // product authentication
    //addAndMakeVisible(productLockScreen);
    addChildComponent(productLockScreen);

    // TODO: implement auto activation checking

    productLockScreen.reactivate();

    showUnlockForm();
    
}

SynergyAudioProcessorEditor::~SynergyAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void SynergyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(30,30,30));

    // background image
    Image backgroundImage;
    switch (theme->themeID) 
    {
        case 1:
            // default theme
            backgroundImage = juce::ImageCache::getFromMemory(BinaryData::SynergyBackgroundV1transformed_png, BinaryData::SynergyBackgroundV1transformed_pngSize);
            break;
        case 2:
            // black and white theme
            backgroundImage = juce::ImageCache::getFromMemory(BinaryData::SynergyBackgroundWhite_png, BinaryData::SynergyBackgroundWhite_pngSize);
            break;
    }

    g.drawImageWithin(backgroundImage, 0, 0, this->getWidth(), this->getHeight(), juce::RectanglePlacement::stretchToFit);
    

    Image settingsBorderImage = ImageCache::getFromMemory(BinaryData::settingsBordertransformed_png, BinaryData::settingsBordertransformed_pngSize);
    g.drawImageWithin(settingsBorderImage, 190, 280, settingsBorderImage.getWidth() - 190, settingsBorderImage.getHeight() - 190, RectanglePlacement::centred);

    // slider border
    g.setColour(Colour(20, 20, 20));
    //g.drawRect(120, 195, 110, 25, 5);

    g.setColour (Colour(100,100,100));
    g.setFont (15.0f);

    // version text
    string versionString = "Execution Log           Build v";
    if (developmentMode) g.drawFittedText(versionString.append(ProjectInfo::versionString), 0, 0, getWidth(), 30, juce::Justification::topRight, 1);
    
    
}

void SynergyAudioProcessorEditor::resized()
{
    // product lock screen
    productLockScreen.setBounds(0, 0, 900, 700);

    // sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);

    auto halfWidth = getWidth() / 2;
    auto buttonsBounds = getLocalBounds().withWidth(halfWidth).reduced(10);
    bassDrumButton.setBounds(buttonsBounds.getX(), 10, buttonsBounds.getWidth(), 20);

    // dev tools
    messageBox.setBounds(690, 20, 200, 100);
    devModeLabel.setBounds(0, 0, 150, 15);

    // select key combo
    selectKeyCombo.setBounds(660, 260, 140, 40);

    // stem type combo
    stemTypeCombo.setBounds(100, 260, 140, 40);

    // generate button
    generateButton.setBounds(255, 130, 400, 200);

    // midi file drop
    midiFileDrop.setBounds(100, 410, 700, 200);

    // note velocity slider
    noteVelocitySlider.setBounds(62, 160, 127, 15);
    velocitySliderOverlay.setBounds(62, 160, 127, 20);
    noteVelocityValue.setBounds(100, 180, 50, 10);

    // variety slider
    varietySlider.setBounds(696, 165, 127, 15);
    varietySliderOverlay.setBounds(696, 165, 127, 20);
    varietySliderValue.setBounds(734, 185, 50, 10);
    
    // settings button
    settingsButton.setBounds(375, 347, 35, 35);

    // preview button
    previewButton.setBounds(480, 348, 32, 32);

    // record button
    recordButton.setBounds(428, 347, 35, 35);
}

void SynergyAudioProcessorEditor::openSettings() {

    // extra security measure
    verifyPluginIsActivated();

    auto settings = new Settings();
    settings->setComponentID("Settings");
    settings->setUsingNativeTitleBar(true);
    settings->centreWithSize(500, 400);
    settings->setVisible(true);
    
}

void SynergyAudioProcessorEditor::verifyPluginIsActivated()
{
    if (!productUnlockStatus.isUnlocked()) showUnlockForm();
}

void SynergyAudioProcessorEditor::timerCallback() 
{
    if (!isUnlocked && productUnlockStatus.isUnlocked())
    {
        isUnlocked = true;
        unlockPlugin();
    }
}

void SynergyAudioProcessorEditor::unlockPlugin()
{
    productLockScreen.dismiss();
}

void SynergyAudioProcessorEditor::showUnlockForm() 
{
    productLockScreen.setVisible(true);
}