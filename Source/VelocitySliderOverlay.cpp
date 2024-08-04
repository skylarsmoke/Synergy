/*
  ==============================================================================

    VelocitySliderOverlay.cpp
    Created: 15 May 2024 9:09:51pm
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VelocitySliderOverlay.h"

//==============================================================================
VelocitySliderOverlay::VelocitySliderOverlay()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
}

VelocitySliderOverlay::~VelocitySliderOverlay()
{
}

void VelocitySliderOverlay::paint (juce::Graphics& g)
{
    g.setColour (juce::Colour(20,20,20));
    g.fillRect(14, 0, 3, 15);
    g.fillRect(30, 0, 3, 15);
    g.fillRect(46, 0, 3, 15);
    g.fillRect(62, 0, 3, 15);
    g.fillRect(78, 0, 3, 15);
    g.fillRect(94, 0, 3, 15);
    g.fillRect(110, 0, 3, 15);
    
}

void VelocitySliderOverlay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}


