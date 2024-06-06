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
};