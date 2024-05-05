/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <ctime>

using namespace std;
using namespace juce;

void SynergyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.noteOnVel = midiVolume.getValue();
}

//==============================================================================
SynergyAudioProcessorEditor::SynergyAudioProcessorEditor (SynergyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (900, 700);

    // theme object
    theme = new Theme();

    // defining parameters of our slider
    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Volume");
    midiVolume.setValue(1.0);
    midiVolume.setColour(1, juce::Colour(100,100,100));
    midiVolume.addListener(this);
    midiVolume.repaint();
    // add slider to editor
    //addAndMakeVisible(&midiVolume);
}

SynergyAudioProcessorEditor::~SynergyAudioProcessorEditor()
{
}

//==============================================================================
void SynergyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(30,30,30));

    // background image
    Image backgroundImage;
    switch (theme->getThemeID()) 
    {
        case 1:
            // default theme
            backgroundImage = juce::ImageCache::getFromMemory(BinaryData::SynergyBackgroundV1_png, BinaryData::SynergyBackgroundV1_pngSize);
            break;
        case 2:
            // black and white theme
            backgroundImage = juce::ImageCache::getFromMemory(BinaryData::SynergyBackgroundWhite_png, BinaryData::SynergyBackgroundWhite_pngSize);
            break;
    }

    g.drawImageAt(backgroundImage, 0, 0);

    g.setColour (Colour(100,100,100));
    g.setFont (15.0f);

    // version text
    if (developmentMode) g.drawFittedText("Build v0.0.1", 0, 0, getWidth(), 30, juce::Justification::topRight, 1);

    
}

void SynergyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
}
