/*
  ==============================================================================

    MidiDragOutput.cpp
    Created: 11 Jun 2024 10:45:26pm
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MidiDragOutput.h"
#include "GenerateButton.h"

//==============================================================================
MidiDragOutput::MidiDragOutput(GenerateButton& gb) : generateButton(&gb)
{
    synergyFont.setBold(true);
    synergyFont.setHeight(14.0f);
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

MidiDragOutput::~MidiDragOutput()
{
    if (tempFile.existsAsFile())
        tempFile.deleteFile();
}

void MidiDragOutput::paint (juce::Graphics& g)
{
    

    g.setColour(juce::Colour(50, 50, 50).withAlpha(0.4f)); // Set the colour with transparency
    g.fillRoundedRectangle(240, 45, 215, 100, 2.0f);

    g.setOpacity(1.0f);
    g.drawImageWithin(juce::ImageCache::getFromMemory(BinaryData::MidiIcon_png, BinaryData::MidiIcon_pngSize), 320, 60, 50, 50, juce::RectanglePlacement::centred);

    g.setColour(juce::Colour(20, 20, 20));
    g.setFont(synergyFont);
    g.drawSingleLineText("Drag Generated MIDI File", 265, 130);

}

void MidiDragOutput::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void MidiDragOutput::mouseDown(const juce::MouseEvent& event) 
{
    // Create a temporary file to store the MIDI data
    tempFile = juce::File::getSpecialLocation(juce::File::tempDirectory).getChildFile("SynergyBassBassline.mid");
    tempFile.deleteFile();
    tempFile.create();
    juce::FileOutputStream outputStream(tempFile);

    if (generateButton->outputMidiFile.writeTo(outputStream))
    {
        // Start the drag-and-drop operation
        startDragging(tempFile.getFullPathName(), this);
    }
}

void MidiDragOutput::mouseExit(const juce::MouseEvent& event) 
{
    setVisible(false);
}
