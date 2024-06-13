/*
  ==============================================================================

    MidiDragOutput.h
    Created: 11 Jun 2024 10:45:26pm
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
// Forward declaration
class GenerateButton;

//==============================================================================
/*
*/
class MidiDragOutput  : public juce::Component, public juce::DragAndDropContainer
{
public:
    MidiDragOutput(GenerateButton& gb);
    ~MidiDragOutput() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;

    void mouseExit(const juce::MouseEvent& event) override;
    

private:
    GenerateButton* generateButton;
    juce::File tempFile;
    juce::Font synergyFont;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiDragOutput)
};
