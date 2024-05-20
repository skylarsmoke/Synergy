/*
  ==============================================================================

    StemTypeCombo.cpp
    Created: 14 May 2024 9:09:27pm
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "StemTypeCombo.h"

//==============================================================================
StemTypeCombo::StemTypeCombo()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(200, 40);
    setLookAndFeel(&synergyLookAndFeel);
    stemTypeCombo.setComponentID("StemType");    
    stemTypeCombo.addItem("Drums", 1);
    stemTypeCombo.addItem("Chords", 2);
    stemTypeCombo.addItem("Melody", 3);
    stemTypeCombo.setSelectedId(1);
    stemTypeCombo.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    addAndMakeVisible(stemTypeCombo);

}

StemTypeCombo::~StemTypeCombo()
{
    setLookAndFeel(nullptr);
}

void StemTypeCombo::paint (juce::Graphics& g)
{
    
}

void StemTypeCombo::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    stemTypeCombo.setBounds(0, 2, 140, 30);

}

bool StemTypeCombo::hitTest(int x, int y) {
    return hitArea.contains(x, y);
}
