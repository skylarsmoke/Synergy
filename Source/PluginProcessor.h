/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Bass1.h"
#include "Bass1Voice.h"
#include "MidiNote.h"
#include "MidiViewer.h"

//==============================================================================
/**
*/
class SynergyAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SynergyAudioProcessor();
    ~SynergyAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadFile(const juce::String& path);
    
    /// <summary>
    /// Loads the midi sequence created by the markov chain
    /// </summary>
    /// <param name="midiSeq"></param>
    void loadPreviewMidiFile(const juce::MidiMessageSequence midiSeq);

    /// <summary>
    /// Plays the preview audio for the current midi
    /// </summary>
    void playAudio();

    /// <summary>
    /// Sets the BPM for previewing
    /// </summary>
    /// <param name="newBPM">BPM</param>
    void setBPM(double newBPM);

    /// <summary>
    /// Sets the preview bass sound on the synth
    /// </summary>
    /// <param name="previewBass"></param>
    void setPreviewBass(int previewBass);

    /// <summary>
    /// Gets the BPM of a Midi File
    /// </summary>
    /// <param name="midiFile"></param>
    /// <returns></returns>
    double getMidiFileBPM();


    // variables
    float noteOnVel = 0;
    juce::MidiFile midiFile;

    juce::MidiMessageSequence midiSequence;

    juce::AudioProcessorValueTreeState parameters;
    std::vector<MidiNote> midiNotes;

private:
    juce::Synthesiser synth;
    double currentPosition = 0.0;
    juce::AudioFormatManager formatManager;
    double sampleRate = 48000;
    bool isPlaying = false;
    bool isPreviewing = false;
    bool wasPlaying = false;
    int currentEventIndex = 0;
    double bpm = 175.0;  // Default BPM
    int ppq = 960;       // Default PPQ

    /// <summary>
    /// updates the preview BPM from the DAW
    /// </summary>
    void updateBPMFromHost();

    void fadeOutBuffer(juce::AudioBuffer<float>& buffer);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynergyAudioProcessor)
};
