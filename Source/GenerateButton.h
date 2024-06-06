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
#include "MidiViewer.h"
#include "BassGenerator.h"

//==============================================================================
/*
*/
class GenerateButton  : public juce::Component
{
public:
    GenerateButton(BassGenerator& bassAI, MidiViewer& midiV, ComboBox& stemT, ComboBox& key, Viewport& viewP);
    ~GenerateButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // allows the generate button to only be clickable on the button
    bool hitTest(int x, int y) override;

    juce::ImageButton generateButton;

private:
    juce::Rectangle<int> hitArea{ 100, 60, 180, 100 };
    BassGenerator* bassGenerator;
    MidiViewer* midiViewer;
    Viewport* viewport;
    ComboBox* stemType;
    ComboBox* musicalKey;


    /// <summary>
    /// Calls logic when the generate button is clicked
    /// </summary>
    void generate();
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerateButton)
};
