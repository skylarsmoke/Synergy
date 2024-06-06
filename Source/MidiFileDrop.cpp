/*
  ==============================================================================

    MidiFileDrop.cpp
    Created: 11 May 2024 3:22:06pm
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MidiFileDrop.h"

//==============================================================================
MidiFileDrop::MidiFileDrop(SynergyAudioProcessor& p, juce::TextEditor& messageBox) : audioProcessor (p), messageBox (messageBox)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    midiLoaded = false;

    synergyFont.setBold(true);
    synergyFont.setHeight(14.0f);
}

MidiFileDrop::~MidiFileDrop()
{
}

void MidiFileDrop::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */


    // rectangle styling
    /*g.setColour(juce::Colour(30, 30, 30));
    g.fillRect(4, 4, 192, 192);*/

    if (!midiLoaded) {
        g.drawImageWithin(juce::ImageCache::getFromMemory(BinaryData::MidiIcon_png, BinaryData::MidiIcon_pngSize), 320, 60, 50, 50, juce::RectanglePlacement::centred);
        
        g.setColour(juce::Colour(20, 20, 20));
        g.setFont(synergyFont);
        g.drawSingleLineText("Drop MIDI File", 296, 130);
    }
    else
    {
        g.drawImageWithin(juce::ImageCache::getFromMemory(BinaryData::CheckMarkIcon_png, BinaryData::CheckMarkIcon_pngSize), 320, 60, 50, 50, juce::RectanglePlacement::centred);

        g.setColour(juce::Colour(20, 20, 20));
        g.setFont(synergyFont);
        g.drawSingleLineText("MIDI Loaded", 302, 130);
    }

    

}

void MidiFileDrop::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

// decides if the file type of the dragged file is what we want
bool MidiFileDrop::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".mid"))
        {
            logMessage("Midi file recognized.");
            return true;
        }
    }

    logMessage("File not a midi file.");
    return false;
}


void MidiFileDrop::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(files))
        {
            // load the file
            audioProcessor.loadFile(file);
            logMessage("Midi file loading complete.");
            midiLoaded = true;
            this->repaint();
        }
    }
}
