/*
  ==============================================================================

    SelectKeyComboBox.cpp
    Created: 15 May 2024 1:24:52pm
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SelectKeyComboBox.h"

//==============================================================================
SelectKeyComboBox::SelectKeyComboBox()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(200, 40);
    setLookAndFeel(&synergyLookAndFeel);

    selectKeyCombo.addItem("Ab Major", 1);
    selectKeyCombo.addItem("A Major", 2);
    selectKeyCombo.addItem("Bb Major", 3);
    selectKeyCombo.addItem("B Major", 4);
    selectKeyCombo.addItem("C Major", 5);
    selectKeyCombo.addItem("Db Major", 6);
    selectKeyCombo.addItem("D Major", 7);
    selectKeyCombo.addItem("Eb Major", 8);
    selectKeyCombo.addItem("E Major", 9);
    selectKeyCombo.addItem("F Major", 10);
    selectKeyCombo.addItem("Gb Major", 11);
    selectKeyCombo.addItem("G Major", 12);
    selectKeyCombo.addItem("A Minor", 13);
    selectKeyCombo.addItem("Bb Minor", 14);
    selectKeyCombo.addItem("B Minor", 15);
    selectKeyCombo.addItem("C Minor", 16);
    selectKeyCombo.addItem("Db Minor", 17);
    selectKeyCombo.addItem("D Minor", 18);
    selectKeyCombo.addItem("Eb Minor", 19);
    selectKeyCombo.addItem("E Minor", 20);
    selectKeyCombo.addItem("F Minor", 21);
    selectKeyCombo.addItem("Gb Minor", 22);
    selectKeyCombo.addItem("G Minor", 23);
    selectKeyCombo.addItem("Ab Minor", 24);
    selectKeyCombo.setSelectedId(5);
    selectKeyCombo.setComponentID("Key");
    selectKeyCombo.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    selectKeyCombo.setTooltip("The musical key you want the bassline in.");
    addAndMakeVisible(selectKeyCombo);
}

SelectKeyComboBox::~SelectKeyComboBox()
{
    setLookAndFeel(nullptr);
}

void SelectKeyComboBox::paint (juce::Graphics& g)
{
    
}

void SelectKeyComboBox::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    selectKeyCombo.setBounds(0, 2, 140, 30);
}
