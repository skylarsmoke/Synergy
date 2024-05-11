/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <ctime>

using namespace std;
using namespace juce;

void SynergyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.noteOnVel = midiVolume.getValue();
}

//==============================================================================
SynergyAudioProcessorEditor::SynergyAudioProcessorEditor (SynergyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), startTime (Time::getMillisecondCounterHiRes() * 0.001) 
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (900, 700);
    synergyFont.setBold(true);
    synergyFont.setHeight(14.0f);
    //synergyFont.setTypefaceName("Stars Fighters");

    // theme object
    theme = new Theme();
    setLookAndFeel(&synergyLookAndFeel);

    // dev mode label
    devModeLabel.setText("DEVELOPMENT MODE", NotificationType::dontSendNotification);
    devModeLabel.setColour(Label::textColourId, Colour(100,100,100));
    devModeLabel.setFont(synergyFont);
    if (developmentMode) addAndMakeVisible(devModeLabel);

    // generate button
    generateButton.setButtonText("Generate");
    addAndMakeVisible(generateButton);

    // stem type combo box
    stemTypeLabel.setText("Stem Type", NotificationType::dontSendNotification);
    stemTypeLabel.setColour(Label::textColourId, theme->mainTextColor);
    stemTypeLabel.setFont(synergyFont);
    addAndMakeVisible(stemTypeLabel);

    stemTypeCombo.addItem("Drums", 1);
    stemTypeCombo.addItem("Chords", 2);
    stemTypeCombo.addItem("Melody", 3);
    stemTypeCombo.setSelectedId(1);
    /*stemTypeCombo.setColour(ComboBox::backgroundColourId, theme->mainButtonColor);
    stemTypeCombo.setColour(ComboBox::textColourId, theme->mainTextColor);
    stemTypeCombo.setColour(ComboBox::outlineColourId, theme->mainButtonColor);
    stemTypeCombo.setColour(ComboBox::arrowColourId, theme->mainTextColor);*/
    addAndMakeVisible(stemTypeCombo);

    // select key combo box
    keyLabel.setText("Key", NotificationType::dontSendNotification);
    keyLabel.setColour(Label::textColourId, theme->mainTextColor);
    keyLabel.setFont(synergyFont);
    addAndMakeVisible(keyLabel);

    selectKeyCombo.addItem("Ab Major", 1);
    selectKeyCombo.addItem("A Major", 2);
    selectKeyCombo.addItem("Bb Major", 3);
    selectKeyCombo.addItem("B Major", 4);
    selectKeyCombo.addItem("C Major", 5);
    selectKeyCombo.addItem("Db Major", 6);
    selectKeyCombo.addItem("D Major", 7);
    selectKeyCombo.addItem("Eb Major", 8);
    selectKeyCombo.addItem("E Major", 9);
    selectKeyCombo.addItem("F Major", 10);
    selectKeyCombo.addItem("F# Major", 11);
    selectKeyCombo.addItem("G Major", 12);
    selectKeyCombo.addItem("A Minor", 13);
    selectKeyCombo.addItem("Bb Minor", 14);
    selectKeyCombo.addItem("B Minor", 15);
    selectKeyCombo.addItem("C Minor", 16);
    selectKeyCombo.addItem("C# Minor", 17);
    selectKeyCombo.addItem("D Minor", 18);
    selectKeyCombo.addItem("D# Minor", 19);
    selectKeyCombo.addItem("E Minor", 20);
    selectKeyCombo.addItem("F Minor", 21);
    selectKeyCombo.addItem("F# Minor", 22);
    selectKeyCombo.addItem("G Minor", 23);
    selectKeyCombo.addItem("G# Minor", 24);
    selectKeyCombo.setSelectedId(5);

    /*selectKeyCombo.setColour(ComboBox::backgroundColourId, theme->mainButtonColor);
    selectKeyCombo.setColour(ComboBox::textColourId, theme->mainTextColor);
    selectKeyCombo.setColour(ComboBox::outlineColourId, theme->mainButtonColor);
    selectKeyCombo.setColour(ComboBox::arrowColourId, theme->mainTextColor);*/
    /*auto test = selectKeyCombo.getRootMenu();
    test.set
    selectKeyCombo.setColour(selectKeyCombo.getRootMenu()->backgroundColourId, Colour(30, 30, 30));*/
    
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
            backgroundImage = juce::ImageCache::getFromMemory(BinaryData::SynergyBackgroundV1_png, BinaryData::SynergyBackgroundV1_pngSize);
            break;
        case 2:
            // black and white theme
            backgroundImage = juce::ImageCache::getFromMemory(BinaryData::SynergyBackgroundWhite_png, BinaryData::SynergyBackgroundWhite_pngSize);
            break;
    }

    g.drawImageAt(backgroundImage, 0, 0);

    g.setColour (Colour(100,100,100));
    g.setFont (15.0f);

    // version text
    if (developmentMode) g.drawFittedText("Build v0.0.8", 0, 0, getWidth(), 30, juce::Justification::topRight, 1);

    
}

void SynergyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);

    auto halfWidth = getWidth() / 2;
    auto buttonsBounds = getLocalBounds().withWidth(halfWidth).reduced(10);
    bassDrumButton.setBounds(buttonsBounds.getX(), 10, buttonsBounds.getWidth(), 20);

    // dev tools
    messageBox.setBounds(690, 20, 200, 100);
    devModeLabel.setBounds(0, 0, 150, 15);

    // select key combo
    selectKeyCombo.setBounds(645, 205, 100, 30);
    keyLabel.setBounds(selectKeyCombo.getBounds().getX() + (selectKeyCombo.getWidth() / 3) - 2, 90, 200, 200);

    // stem type combo
    stemTypeCombo.setBounds(160, 205, 100, 30);
    stemTypeLabel.setBounds(stemTypeCombo.getBounds().getX() + 10, 90, 200, 200);

    // generate button
    generateButton.setBounds(400, 220, 100, 30);
    
}
