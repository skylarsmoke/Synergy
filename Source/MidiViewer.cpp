/*
  ==============================================================================

    MidiViewer.cpp
    Created: 29 May 2024 8:57:35pm
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MidiViewer.h"

//==============================================================================
MidiViewer::MidiViewer(int numberOfOctaves) : numOctaves(numberOfOctaves), cellWidth(20), cellHeight(20)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(672, 200);
    noteNames = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
}

MidiViewer::~MidiViewer()
{
}

void MidiViewer::paint (juce::Graphics& g)
{
    // Clear the background
    g.fillAll(juce::Colour(30, 30, 30));

    if (midiNotes.empty()) return;

    const int beatsPerBar = 4;
    const int totalHeight = cellHeight * 12 * numOctaves;

    // Calculate the range of notes
    int minPitch = 127;
    int maxPitch = 0;
    float maxEndBeat = 0.0f;

    for (const auto& note : midiNotes)
    {
        if (note.pitch < minPitch) minPitch = note.pitch;
        if (note.pitch > maxPitch) maxPitch = note.pitch;
        float endBeat = note.startBeat + note.lengthInBeats;
        if (endBeat > maxEndBeat) maxEndBeat = endBeat;
    }

    int pitchRange = maxPitch - minPitch + 1;
    int totalBeats = static_cast<int>(std::ceil(maxEndBeat));
    int totalBars = (totalBeats + beatsPerBar - 1) / beatsPerBar;

    // Adjust cell size to fit all notes
    cellWidth = getWidth() / (totalBars * beatsPerBar);
    cellHeight = getHeight() / pitchRange;

    // Recalculate grid dimensions
    int gridWidth = totalBars * beatsPerBar * cellWidth;
    int gridHeight = pitchRange * cellHeight;

    // Draw horizontal rows with different colors for sharps/flats and naturals
    juce::Colour naturalNoteColour = juce::Colour::fromRGB(40, 40, 40);
    juce::Colour sharpFlatNoteColour = juce::Colour::fromRGB(30, 30, 30);
    juce::StringArray sharpFlatNotes = { "C#", "D#", "F#", "G#", "A#" };

    for (int i = 0; i < pitchRange; ++i)
    {
        int y = i * cellHeight;
        int pitch = minPitch + i;
        int pitchInOctave = pitch % 12;

        juce::String noteName = noteNames[pitchInOctave];
        bool isSharpFlat = sharpFlatNotes.contains(noteName);

        if (isSharpFlat)
        {
            g.setColour(sharpFlatNoteColour);
        }
        else
        {
            g.setColour(naturalNoteColour);
        }

        g.fillRect(0, y, gridWidth, cellHeight);
    }

    g.setColour(juce::Colour::fromRGB(20, 20, 20));

    // Draw vertical grid lines (for beats)
    for (int i = 0; i <= totalBars * beatsPerBar; ++i)
    {
        int x = i * cellWidth;
        if (i % beatsPerBar == 0 && i != 0)
        {
            g.drawLine(x, 0, x, gridHeight, 2.0f);
        }
        else
        {
            g.drawLine(x, 0, x, gridHeight, 1.0f);
        }
    }

    // Draw horizontal grid lines (for notes)
    for (int i = 0; i <= pitchRange; ++i)
    {
        int y = i * cellHeight;

        if (i % 12 == 0 && i != 0)
        {
            g.drawLine(0, y - 1, gridWidth, y - 1, 2.0f);
        }
        else
        {
            g.drawLine(0, y, gridWidth, y, 1.0f);
        }
    }

    // Draw the note names or any other labels
    juce::Font noteFont(10.0f, juce::Font::plain);
    g.setFont(noteFont);
    g.setColour(juce::Colour::fromRGB(80, 80, 80));
    for (int i = 0; i < pitchRange; ++i)
    {
        int pitch = minPitch + i;
        int octave = pitch / 12;
        int pitchInOctave = pitch % 12;
        int y = (pitchRange - 1 - i) * cellHeight;
        g.drawText(noteNames[pitchInOctave] + juce::String(octave), 2, y, cellWidth - 2, cellHeight, juce::Justification::centredLeft);
    }

    // Draw beat markers at the top
    g.setColour(juce::Colour::fromRGB(80, 80, 80));
    for (int i = 1; i <= totalBars * beatsPerBar; ++i)
    {
        int x = i * cellWidth;
        if (i % beatsPerBar == 0)
        {
            g.drawText(juce::String(i / beatsPerBar + 1), x, 0, cellWidth, cellHeight, juce::Justification::centred);
        }
    }

    // Draw MIDI notes
    for (const auto& note : midiNotes)
    {
        g.setColour(juce::Colour(110, 88, 255));
        // Assuming the length is already in beats, no need to convert
        float startBeat = note.startBeat;
        float lengthInBeats = note.lengthInBeats;

        int x = static_cast<int>(startBeat * cellWidth);
        int pitch = note.pitch - minPitch;
        int y = (pitchRange - 1 - pitch) * cellHeight;
        int width = static_cast<int>(lengthInBeats * cellWidth);
        int height = cellHeight;

        g.fillRect(x, y, width, height);

        // Draw the border
        g.setColour(juce::Colours::black);
        g.drawRect(x, y, width, height, 1);
    }
}

void MidiViewer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..A


}

void MidiViewer::setMidiNotes(const std::vector<MidiNote>& notes)
{
    midiNotes = notes;
    if (!midiNotes.empty())
    {
        maxBeats = std::max_element(midiNotes.begin(), midiNotes.end(),
                                    [](const MidiNote& a, const MidiNote& b)
                                    {
                                        return a.startBeat + a.lengthInBeats < b.startBeat + b.lengthInBeats;
                                    })->startBeat / 1000.0f * 2.0f + std::max_element(midiNotes.begin(), midiNotes.end(),
                                                                                      [](const MidiNote& a, const MidiNote& b)
                                                                                      {
                                                                                          return a.lengthInBeats < b.lengthInBeats;
                                                                                      })->lengthInBeats / 1000.0f * 2.0f;
    }
    repaint();
}
