/*
  ==============================================================================

    MidiNote.h
    Created: 4 Jun 2024 10:35:17pm
    Author:  skyla

  ==============================================================================
*/

#pragma once


struct MidiNote
{
    int pitch;        // MIDI pitch (0-127)
    int octave;       // Octave number
    float startBeat;  // Start time in beats
    float lengthInBeats; // Duration in beats

    juce::String toString() const
    {
        return juce::String(pitch) + "," + juce::String(octave) + "," +
            juce::String(startBeat) + "," + juce::String(lengthInBeats);
    }
    
    static MidiNote fromString(const juce::String& str)
    {
        MidiNote note;
        auto tokens = juce::StringArray::fromTokens(str, ",", "");
        if (tokens.size() == 4)
        {
            note.pitch = tokens[0].getIntValue();
            note.octave = tokens[1].getIntValue();
            note.startBeat = tokens[2].getFloatValue();
            note.lengthInBeats = tokens[3].getFloatValue();
        }
        return note;
    }
};