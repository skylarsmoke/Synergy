#pragma once

/*
* This look and feel class overrides the core JUCE look and feel to give it the custom synergy look.
*/

#include "JuceHeader.h"

class SynergyLookAndFeel : public juce::LookAndFeel_V4
{
public:
	SynergyLookAndFeel();

	void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
							  bool, bool isButtonDown) override
	{
		auto buttonArea = button.getLocalBounds();
		g.setColour(backgroundColour);
		g.fillRect(buttonArea);

		if (isButtonDown) 
		{
			g.setColour(button.findColour(juce::TextButton::buttonOnColourId));
			g.fillRect(buttonArea);
		}
	}

	void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
	{
		g.setColour(button.findColour(juce::TextButton::textColourOffId));
		g.drawFittedText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, 1);
	}

	void drawComboBoxTextWhenNothingSelected(juce::Graphics& g, juce::ComboBox& comboBox, juce::Label& label) override
	{
		g.setColour(comboBox.findColour(juce::ComboBox::textColourId));
		
		
	}

	void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttomW, int buttonH, juce::ComboBox& comboBox) override 
	{
		auto comboArea = comboBox.getLocalBounds();
		g.setColour(comboBox.findColour(juce::ComboBox::backgroundColourId));
		
		g.fillRect(comboArea);

		

		if (isButtonDown || comboBox.isPopupActive() || comboBox.isMouseOver()) {
			g.setColour(juce::Colour(40, 40, 40));
			g.fillRect(comboArea);
		}
	}

	void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override
	{
		g.setColour(juce::Colour(20, 20, 20));
		g.fillRect(0, 0, width, height);
	}

	void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle< int >& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText, const juce::Drawable* icon, const juce::Colour* textColour) override
	{
		g.setColour(juce::Colour(20, 20, 20));
		g.fillRect(area);
		g.setFont(juce::Font(16.0f));

		if (isHighlighted || isTicked) 
		{
			g.setColour(juce::Colour(30,30,30));
			g.fillRect(area.reduced(2, 2));
		}

		g.setColour(juce::Colour(150,150,150));
		g.drawText(text, area.reduced(10, 0), juce::Justification::left);
		
	}

	void getIdealPopupMenuItemSize(const juce::String& text, const bool isSeparator, int standardMenuItemHeight, int& idealWidth, int& idealHeight) override
	{
		idealWidth = 10;
		idealHeight = 25;
	}

};

