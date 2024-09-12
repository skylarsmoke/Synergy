/*
  ==============================================================================

    GenerateButton.h
    Created: 14 May 2024 12:12:00am
    Author:  skyla

    Contains code pertaining to the generate button

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "BassGenerator.h"
#include "PluginProcessor.h"
#include "SettingsCache.h"


// Forward declaration
class MidiViewer;

//==============================================================================
/*
*/
class GenerateButton  : public juce::Component, private juce::Timer
{
public:
    GenerateButton(BassGenerator& bassAI, 
                   MidiViewer& midiV, 
                   ComboBox& stemT, 
                   ComboBox& key, 
                   Viewport& viewP, 
                   Slider& varietySliderV,
                   SynergyAudioProcessor&,
                   SettingsCache&,
                   Slider&,
                   Slider&);

    ~GenerateButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // allows the generate button to only be clickable on the button
    bool hitTest(int x, int y) override;

    juce::ImageButton generateButton;

    juce::MidiFile outputMidiFile;


    /// <summary>
    /// Creates a midi file from a midi notes vector
    /// </summary>
    /// <param name="notes">Notes</param>
    /// <param name="inputMidiFile">Input Midi File</param>
    /// <param name="outputFile">Output File</param>
    void createMidiFile(const std::vector<MidiNote>& notes, const juce::MidiFile& inputMidiFile);

private:
    juce::Rectangle<int> hitArea{ 100, 60, 180, 100 };
    BassGenerator* bassGenerator;
    MidiViewer* midiViewer;
    Viewport* viewport;
    ComboBox* stemType;
    ComboBox* musicalKey;
    Slider* varietySlider;
    Slider* velocitySlider;
    Slider* swingSlider;
    SynergyAudioProcessor& audioProcessor;
    SettingsCache* settingsCache;

    // New label for showing the midi not loaded message
    juce::Label warningMessageLabel;


    /// <summary>
    /// Calls logic when the generate button is clicked
    /// </summary>
    void generate();


    /// <summary>
    /// Sets the bassline loop from the settings cache
    /// </summary>
    /// <returns></returns>
    int setBasslineLoop();

    /// <summary>
    /// Shows a message when attempting to generate without a midi loaded
    /// </summary>
    /// <param name="message">Message</param>
    void showWarningMessage(const juce::String& message);

    void timerCallback() override;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerateButton)
};
