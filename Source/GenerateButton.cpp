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
                               Slider& noteV,
                               Slider& swing) :
    bassGenerator(&bassAI), 
    midiViewer(&midiV), 
    stemType(&stemT), 
    musicalKey(&key), 
    viewport(&viewP), 
    varietySlider(&varietySliderV),
    audioProcessor(p),
    settingsCache(&sc),
    velocitySlider(&noteV),
    swingSlider(&swing)
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

    // add and configure the warning message label
    warningMessageLabel.setText("", juce::dontSendNotification);
    warningMessageLabel.setColour(Label::textColourId, juce::Colour(100, 100, 100));
    warningMessageLabel.setJustificationType(Justification::centred);
    warningMessageLabel.setAlpha(0.0f);  // Initially hidden
    addAndMakeVisible(warningMessageLabel);
}

GenerateButton::~GenerateButton()
{
}

void GenerateButton::paint (juce::Graphics& g)
{
    // Draw the rounded rectangle around the warning message label
    if (warningMessageLabel.getAlpha() > 0.0f)  // Only draw if the label is visible
    {
        float cornerSize = 3.0f;   // Radius for the rounded corners
        auto labelBounds = warningMessageLabel.getBounds().toFloat().reduced(5.0f);  // Add some padding around the label

        // Set the fill color for the rounded rectangle (e.g., semi-transparent yellow)
        g.setColour(juce::Colour(20, 20, 20).withAlpha(1.0f));  // 80% opacity
        g.fillRoundedRectangle(labelBounds, cornerSize);

        // Set the color for the border and draw it
        g.setColour(juce::Colour(20, 20, 20).withAlpha(1.0f));  // Border color
        g.drawRoundedRectangle(labelBounds, cornerSize, 2.0f);  // 2.0f is the line thickness
    }
    
}

void GenerateButton::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    generateButton.setBounds(10, 10, 355, 200);

    warningMessageLabel.setBounds(15, 10, 355, 40); // Position the warning message below the button
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
    if (!isMidiFileValid(audioProcessor.midiFile) && stemType->getText() != "None") 
    {
        showWarningMessage("A MIDI file must be loaded to use this stem type.");
        return; // if the file is not valid, we do not generate
    }
    outputMidiFile.clear();

    int basslineLoopLength = setBasslineLoop();
    
    auto newBassline = bassGenerator->generateBassline(audioProcessor.midiFile, stemType->getText(), musicalKey->getText().toStdString(), (int)varietySlider->getValue(), basslineLoopLength, (int)velocitySlider->getValue(), (int)swingSlider->getValue());

    createMidiFile(newBassline, audioProcessor.midiFile);

    if (newBassline.empty()) return;

    midiViewer->setMidiNotes(newBassline);
    audioProcessor.midiNotes = newBassline; // set the midi notes in the audio processor for state information purposes
    viewport->setVisible(true);

}

int GenerateButton::setBasslineLoop()
{
    switch (settingsCache->activeLoopLength)
    {
    case 1:
        //ID for 2 bars
        return 2;
    case 2:
        // ID for 4 bars
        return 4;
    case 3:
        // ID for 2 bars
        return 8;
    default:
        std::cerr << "Bassline loop value not recognized";
    }
}

void GenerateButton::showWarningMessage(const String& message)
{
    warningMessageLabel.setText(message, dontSendNotification);
    warningMessageLabel.setAlpha(1.0f);
    warningMessageLabel.setVisible(true);
    warningMessageLabel.repaint();
    startTimer(2000); // Start a timer to hide the message after 2 seconds
}

void GenerateButton::timerCallback()
{
    warningMessageLabel.setAlpha(0.0f);  // Hide the message after 2 seconds
    warningMessageLabel.repaint();  // Force repaint after changing the alpha
    stopTimer();  // Stop the timer
}








