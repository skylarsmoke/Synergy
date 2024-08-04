/*
  ==============================================================================

    VelocitySliderOverlay.h
    Created: 15 May 2024 9:09:51pm
    Author:  skyla

    contains graphics code that creates a overlay over the velocity slider

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class VelocitySliderOverlay  : public juce::Component
{
public:
    VelocitySliderOverlay();
    ~VelocitySliderOverlay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    bool hitTest(int x, int y) override
    {
        return false;
    }


private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VelocitySliderOverlay)
};
