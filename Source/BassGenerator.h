/*
  ==============================================================================

    BassGenerator.h
    Created: 25 May 2024 11:54:04pm
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <string>
#include "MidiNote.h"

using namespace juce;

//==============================================================================
/*
*/
class BassGenerator  : public juce::Component
{
public:
    BassGenerator();

    /// <summary>
    /// Calls all functions required to train the AI algorithm
    /// </summary>
    /// <param name="midiFile">Midi file to train with</param>
    void train(const MidiFile& midiFile);

    /// <summary>
    /// Trains the AI algorithm from all MIDIs in a given folder
    /// </summary>
    /// <param name="folderPath">Folder containing MIDI files</param>
    void trainFromFolder(const String& folderPath);

    /// <summary>
    /// Generates a bassline from the given stem type
    /// </summary>
    /// <param name="inputMidiFile">The melody midi</param>
    /// <returns></returns>
    std::vector<MidiNote> generateBassline(const MidiFile& inputMidiFile, 
                                           const String& stemType, 
                                           const std::string& inputMusicalKey, 
                                           int noteVariety,
                                           int loopLength,
                                           int noteVelocity);

    

private:
    /// <summary>
    /// Trains the Markov Chain
    /// </summary>
    /// <param name="notes">Midi Notes</param>
    void trainChain(const std::vector<MidiNote>& notes);

    /// <summary>
    /// For each key
    /// </summary>
    std::map<int, std::vector<MidiNote>> markovChain;

    /// <summary>
    /// Exracts the notes from the midi file
    /// </summary>
    /// <param name="midiFile"></param>
    /// <returns></returns>
    std::vector<MidiNote> extractMidiNotes(const MidiFile& midiFile);

    /// <summary>
    /// Generates a bassline from a given melody
    /// </summary>
    /// <param name="melodyNotes"></param>
    /// <returns></returns>
    std::vector<MidiNote> generateBasslineFromMelody(const std::vector<MidiNote>& melodyNotes, 
                                                     int noteVariety,
                                                     int loopLength,
                                                     int noteVelocity);

    /// <summary>
    /// Generates a bassline from a given chord progression
    /// </summary>
    /// <param name="chordNotes"></param>
    /// <returns></returns>
    std::vector<MidiNote> generateBasslineFromChords(const std::vector<MidiNote>& chordNotes,
                                                     int noteVariety,
                                                     int loopLength,
                                                     int noteVelocity);

    /// <summary>
    /// Generates bassline from a given drum midi
    /// </summary>
    std::vector<MidiNote> generateBasslineFromDrums(const std::vector<MidiNote>& drumNotes,
                                                    int noteVariety,
                                                    int loopLength,
                                                    int noteVelocity);

    
    std::vector<MidiNote> generateBasslineNoMidi(int noteVariety,
                                                 int loopLength,
                                                 int noteVelocity);

    /// <summary>
    /// Gets the next note in the chain from the current note
    /// </summary>
    /// <param name="currentNote">Current Note</param>
    /// <returns></returns>
    MidiNote getNextNoteFromChain(int currentNote);

    std::map<std::string, std::vector<int>> scales;
    std::string musicalKey;

    /// <summary>
    /// Initializes the musical scales
    /// </summary>
    void initializeScales();

    /// <summary>
    /// Adjusts the major scale to the given key by using provided semi tones
    /// </summary>
    /// <param name="scale">Major Scale</param>
    /// <param name="semiTones">Semi Tones to adjust by</param>
    /// <returns></returns>
    std::vector<int> adjustScale(const std::vector<int>& scale, int semiTones);

    /// <summary>
    /// Returns whether or not a given note is in the current musical scale
    /// </summary>
    /// <param name="note">Note</param>
    /// <returns></returns>
    bool isNoteInScale(int note);

    /// <summary>
    /// Loads chain data from a file
    /// </summary>
    void loadData();

    /// <summary>
    /// Saves chain data to a file
    /// </summary>
    void saveData(const std::string& pathToPutData);
    
};
