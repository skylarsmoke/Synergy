#include "SynergyLookAndFeel.h"
#include "Theme.h"

/*
* Implementation of the Synergy look and feel class
*/

SynergyLookAndFeel::SynergyLookAndFeel(SettingsCache* sc) : settingsCache(sc)
{
	synergyFont.setBold(true);
	synergyFont.setHeight(16.0f);
	synergyFont.setTypefaceName("Stars Fighters");

	// label
	setColour(juce::Label::textColourId, Colour(100, 100, 100));

	// text button
	setColour(juce::TextButton::buttonColourId, Colour(20, 20, 20));
	setColour(juce::TextButton::buttonOnColourId, Colour(40, 40, 40));
	setColour(juce::TextButton::textColourOffId, Colour(150, 150, 150));

	//setColour(juce::TextButton::)

	// combo box
	setColour(juce::ComboBox::backgroundColourId, Colour(110, 88, 255));
	setColour(juce::ComboBox::textColourId, Colour(20, 20, 20));
	setColour(juce::ComboBox::outlineColourId, Colour(20, 20, 20));
	setColour(juce::ComboBox::arrowColourId, Colour(150, 150, 150));
	
	// slider
	setColour(juce::Slider::textBoxHighlightColourId, Colour(0, 195, 255));
	//etColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

}

SynergyLookAndFeel::~SynergyLookAndFeel() {
	
}

