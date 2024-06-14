/*
  ==============================================================================

    GenerateButton.cpp
    Created: 14 May 2024 12:12:00am
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GenerateButton.h"
#include "MidiViewer.h"

using namespace juce;

//==============================================================================
GenerateButton::GenerateButton(BassGenerator& bassAI, 
                               MidiViewer& midiV, 
                               ComboBox& stemT, 
                               ComboBox& key, 
                               Viewport& viewP, 
                               Slider& varietySliderV,
                               SynergyAudioProcessor& p,
                               SettingsCache& sc,
                               Slider& noteV) :
    bassGenerator(&bassAI), 
    midiViewer(&midiV), 
    stemType(&stemT), 
    musicalKey(&key), 
    viewport(&viewP), 
    varietySlider(&varietySliderV),
    audioProcessor(p),
    settingsCache(&sc),
    velocitySlider(&noteV)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setSize(300, 300);

    // generate button
    Image generateButtonImage = ImageCache::getFromMemory(BinaryData::GenerateButtontransformed_png, BinaryData::GenerateButtontransformed_pngSize);
    Image generateButtonImageHover = ImageCache::getFromMemory(BinaryData::GenerateButtonHovertransformed_png, BinaryData::GenerateButtonHovertransformed_pngSize);
    generateButton.setImages(true, true, true, generateButtonImage, 1.0f, {}, generateButtonImageHover, 1.0f, {}, generateButtonImage, 1.0f, {});
    generateButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    generateButton.onClick = [this] { generate(); };
    generateButton.setTooltip("Generates a bassline based off of the midi and parameters provided.");
    addAndMakeVisible(generateButton);

}

GenerateButton::~GenerateButton()
{
}

void GenerateButton::paint (juce::Graphics& g)
{
    
}

void GenerateButton::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    generateButton.setBounds(10, 10, 355, 200);
}

bool GenerateButton::hitTest(int x, int y)
{
    return hitArea.contains(x, y);
}

void GenerateButton::createMidiFile(const std::vector<MidiNote>&notes, const juce::MidiFile & inputMidiFile) {
    juce::MidiMessageSequence sequence;
    int tpq = 960; // Default to 960 TPQ

    // Search for the tempo track to get ticks per quarter note (TPQ)
    for (int i = 0; i < inputMidiFile.getNumTracks(); ++i) {
        auto* midiTrack = inputMidiFile.getTrack(i);
        for (auto midiEvent : *midiTrack) {
            if (midiEvent->message.isTempoMetaEvent()) {
                tpq = inputMidiFile.getTimeFormat();
                break;
            }
        }
        if (tpq != 960) break; // Break if we have found the TPQ
    }

    for (const auto& note : notes) {
        int tick = static_cast<int>(note.startBeat * tpq);
        int duration = static_cast<int>(note.lengthInBeats * tpq);

        sequence.addEvent(juce::MidiMessage::noteOn(1, note.pitch, (juce::uint8)100), tick);
        sequence.addEvent(juce::MidiMessage::noteOff(1, note.pitch), tick + duration);
    }

    outputMidiFile.setTicksPerQuarterNote(tpq);

    outputMidiFile.addTrack(sequence);
    audioProcessor.loadPreviewMidiFile(sequence);
}

bool isMidiFileValid(const juce::MidiFile& midiFile)
{
    // Check if the MIDI file has tracks and a valid time format
    return midiFile.getTimeFormat() != 0 && midiFile.getNumTracks() > 0;
}

void GenerateButton::generate()
{
    //bassGenerator->trainFromFolder("C:\\Users\\skyla\\OneDrive\\Desktop\\Unison Essential MIDI Basslines");
    
    if (!isMidiFileValid(audioProcessor.midiFile) && stemType->getText() != "None") return; // if the file is not valid, we do not generate
    outputMidiFile.clear();

    int basslineLoopLength = setBasslineLoop();
    
    auto newBassline = bassGenerator->generateBassline(audioProcessor.midiFile, stemType->getText(), musicalKey->getText().toStdString(), (int)varietySlider->getValue(), basslineLoopLength, (int)velocitySlider->getValue());

    createMidiFile(newBassline, audioProcessor.midiFile);

    if (newBassline.empty()) return;

    midiViewer->setMidiNotes(newBassline);
    viewport->setVisible(true);

}

int GenerateButton::setBasslineLoop()
{
    switch (settingsCache->basslineLoop)
    {
    case 1:
        // ID for 4 bars
        return 4;
    case 2:
        // ID for 2 bars
        return 8;
    default:
        std::cerr << "Bassline loop value not recognized";
    }
}








