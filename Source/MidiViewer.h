/*
  ==============================================================================

    MidiViewer.h
    Created: 29 May 2024 8:57:35pm
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiNote.h"

// Forward declaration
class MidiDragOutput;

//==============================================================================
/*
*/
class MidiViewer  : public juce::Component
{
public:

    MidiViewer(MidiDragOutput& md, int numberOfOctaves = 10);
    ~MidiViewer() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setMidiNotes(const std::vector<MidiNote>& notes);


    void mouseEnter(const juce::MouseEvent& event) override;


private:
    int numOctaves;
    std::vector<MidiNote> midiNotes;
    double maxBeats = 0;
    int cellWidth;
    int cellHeight;

    juce::StringArray noteNames;
    MidiDragOutput* midiDragOutput;

    bool isMidiDragOutputVisible = false;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiViewer)
};
