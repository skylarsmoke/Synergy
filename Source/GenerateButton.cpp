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
GenerateButton::GenerateButton()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setSize(300, 300);

    // generate button
    Image generateButtonImage = ImageCache::getFromMemory(BinaryData::GenerateButton_png, BinaryData::GenerateButton_pngSize);
    Image generateButtonImageHover = ImageCache::getFromMemory(BinaryData::GenerateButtonHover_png, BinaryData::GenerateButtonHover_pngSize);
    generateButton.setImages(true, true, true, generateButtonImage, 1.0f, {}, generateButtonImageHover, 1.0f, {}, generateButtonImage, 1.0f, {});
    generateButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
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



