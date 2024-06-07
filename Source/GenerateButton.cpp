/*
  ==============================================================================

    GenerateButton.cpp
    Created: 14 May 2024 12:12:00am
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GenerateButton.h"

using namespace juce;

//==============================================================================
GenerateButton::GenerateButton(BassGenerator& bassAI, 
                               MidiViewer& midiV, 
                               ComboBox& stemT, 
                               ComboBox& key, 
                               Viewport& viewP, 
                               Slider& varietySliderV) : 
    bassGenerator(&bassAI), 
    midiViewer(&midiV), 
    stemType(&stemT), 
    musicalKey(&key), 
    viewport(&viewP), 
    varietySlider(&varietySliderV)
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

void createMidiFile(const std::vector<MidiNote>& notes, const juce::MidiFile& inputMidiFile, const juce::File& outputFile) {
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

    juce::MidiFile outputMidiFile;
    outputMidiFile.setTicksPerQuarterNote(tpq);

    outputMidiFile.addTrack(sequence);

    juce::FileOutputStream outputStream(outputFile);
    if (!outputStream.openedOk()) {
        std::cerr << "Failed to open output file stream." << std::endl;
        return;
    }

    outputMidiFile.writeTo(outputStream);
}

void GenerateButton::generate()
{
    //bassGenerator->trainFromFolder("C:\\Users\\skyla\\OneDrive\\Desktop\\Unison Essential MIDI Basslines");
    
    juce::File midiFile("C:\\Users\\skyla\\OneDrive\\Desktop\\Unison Essential MIDI Basslines\\DHR_125_bass_my_house_Dmin.mid");
    juce::FileInputStream inputStream(midiFile);
    juce::MidiFile juceMidiFile;
    juceMidiFile.readFrom(inputStream);
    
    auto test = bassGenerator->generateBassline(juceMidiFile, stemType->getText(), musicalKey->getText().toStdString(), (int)varietySlider->getValue());

    juce::File outputMidiFilePath("C:\\Users\\skyla\\OneDrive\\Desktop\\test.mid");

    createMidiFile(test, juceMidiFile, outputMidiFilePath);

    midiViewer->setMidiNotes(test);
    viewport->setVisible(true);

}





