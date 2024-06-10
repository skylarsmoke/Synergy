/*
  ==============================================================================

    Bass1.h
    Created: 9 Jun 2024 1:46:39am
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Bass1 : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
};