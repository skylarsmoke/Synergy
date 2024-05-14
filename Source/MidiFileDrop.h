/*
  ==============================================================================

    MidiFileDrop.h
    Created: 11 May 2024 3:22:06pm
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class MidiFileDrop  : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    MidiFileDrop(SynergyAudioProcessor& audioProcessor, juce::TextEditor& messageBox);
    ~MidiFileDrop() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    SynergyAudioProcessor& audioProcessor;
    juce::TextEditor& messageBox;
    bool midiLoaded;

    // main font
    juce::Font synergyFont;

    /// <summary>
    /// Logs data in the message box
    /// </summary>
    /// <param name="m"></param>
    void logMessage(const juce::String& m) {
        messageBox.moveCaretToEnd();
        messageBox.insertTextAtCaret(m + juce::newLine);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiFileDrop)
};
