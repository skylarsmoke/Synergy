#include "SynergyLookAndFeel.h"
#include "Theme.h"

/*
* Implementation of the Synergy look and feel class
*/

SynergyLookAndFeel::SynergyLookAndFeel()
{
	// text button
	setColour(juce::TextButton::buttonColourId, Colour(20, 20, 20));
	setColour(juce::TextButton::buttonOnColourId, Colour(40, 40, 40));
	setColour(juce::TextButton::textColourOffId, Colour(150, 150, 150));

	//setColour(juce::TextButton::)

	// combo box
	setColour(juce::ComboBox::backgroundColourId, Colour(20,20,20));
	setColour(juce::ComboBox::textColourId, Colour(150, 150, 150));
	setColour(juce::ComboBox::outlineColourId, Colour(20, 20, 20));
	setColour(juce::ComboBox::arrowColourId, Colour(150, 150, 150));
	// switch statement to deal with differing theme colors
	//switch (theme->themeID) {
	//case 1:
	//	// default
	//	
	//	break;
	//case 2:
	//	// black and white

	//	break;
	//}

	
}

