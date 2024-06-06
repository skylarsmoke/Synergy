/*
  ==============================================================================

    GenSplash.cpp
    Created: 5 Jun 2024 9:31:30pm
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GenSplash.h"

//==============================================================================
GenSplash::GenSplash() : phase(0.0f)
{
    startTimerHz(60); // Set the frame rate to 60 FPS

}

GenSplash::~GenSplash()
{
    stopTimer();
}

void GenSplash::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();
    int width = area.getWidth();
    int height = area.getHeight();

    // Water wave path
    juce::Path waterPath;
    waterPath.startNewSubPath(0, height / 2);

    float waterAmplitude = 20.0f + std::sin(phase * 0.05f) * 10.0f;

    for (int x = 0; x < width; ++x)
    {
        float y = height / 2 + std::sin((x + phase) * 0.05f) * waterAmplitude;
        y += std::sin((x + phase) * 0.13f) * (waterAmplitude * 0.75f);
        waterPath.lineTo(x, y);
    }

    g.setColour(juce::Colour(0, 195, 255));
    g.strokePath(waterPath, juce::PathStrokeType(2.0f));

    // Electricity wave paths
    for (int i = 0; i < 3; ++i)
    {
        juce::Path electricityPath;
        electricityPath.startNewSubPath(0, height / 2);

        float electricityPhaseOffset = i * 50.0f;
        float electricityAmplitude = 10.0f + std::sin((phase + electricityPhaseOffset) * 0.05f) * 5.0f;

        for (int x = 0; x < width; ++x)
        {
            float y = height / 2 + std::sin((x + phase + electricityPhaseOffset) * 0.1f) * electricityAmplitude;
            y += std::sin((x + phase + electricityPhaseOffset) * 0.25f) * (electricityAmplitude * 0.5f);
            electricityPath.lineTo(x, y);
        }

        g.setColour(juce::Colour(110, 88, 255));
        g.strokePath(electricityPath, juce::PathStrokeType(1.0f));
    }
}

void GenSplash::resized()
{
    
}

void GenSplash::timerCallback()
{
    phase += 1.0f;
    repaint();
}
