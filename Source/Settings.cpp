/*
  ==============================================================================

    Settings.cpp
    Created: 18 May 2024 12:39:33am
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Settings.h"

//==============================================================================
Settings::Settings(SettingsCache& sc, SynergyAudioProcessor& p) : DocumentWindow("Synergy Bass Settings", Colour(20, 20, 20), DocumentWindow::allButtons), settingsCache(&sc), audioProcessor(&p)
{
    settingsInfo = new SettingsInfo(settingsCache, audioProcessor);
    settingsInfo->setVisible(true);
    setContentComponent(settingsInfo);
}

Settings::~Settings()
{
}

void Settings::paint(juce::Graphics& g)
{

}

void Settings::resized()
{
    settingsInfo->setBounds(0, 0, 500, 400);
}
