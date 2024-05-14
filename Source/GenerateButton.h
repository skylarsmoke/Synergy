/*
  ==============================================================================

    GenerateButton.h
    Created: 14 May 2024 12:12:00am
    Author:  skyla

    Contains code pertaining to the generate button

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class GenerateButton  : public juce::Component
{
public:
    GenerateButton();
    ~GenerateButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // allows the generate button to only be clickable on the button
    bool hitTest(int x, int y) override;

private:
    juce::Rectangle<int> hitArea{ 0, 60, 280, 100 };
    juce::ImageButton generateButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerateButton)
};
