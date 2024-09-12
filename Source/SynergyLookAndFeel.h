#pragma once

/*
* This look and feel class overrides the core JUCE look and feel to give it the custom synergy look.
*/

#include "JuceHeader.h"
#include "SettingsCache.h"


class SynergyLookAndFeel : public juce::LookAndFeel_V4
{
public:
	SynergyLookAndFeel(SettingsCache* sc = nullptr);
	~SynergyLookAndFeel();

	juce::Font getComboBoxFont(juce::ComboBox& comboBox) override
	{
		return synergyFont;
	}

	/*juce::Font getPopupMenuFont() override
	{
		return synergyFont;
	}*/

	void drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height) override
	{
		if (!settingsCache->toolTips) return;

		// Background color
		g.fillAll(juce::Colour(15, 15, 15));

		// Text color
		g.setColour(juce::Colour(100,100,100));

		juce::TextLayout textLayout;
		juce::AttributedString attributedText;
		attributedText.setWordWrap(juce::AttributedString::WordWrap::byWord);
		attributedText.setJustification(juce::Justification::centred);
		attributedText.append(text, juce::Font(14.0f), juce::Colour(100, 100, 100));
		textLayout.createLayout(attributedText, static_cast<float>(width - 10)); // Subtract padding

		// Draw text layout
		juce::Rectangle<int> textArea(5, 5, width - 10, height - 10); // Apply padding
		textLayout.draw(g, textArea.toFloat());

		// Border color and width
		g.setColour(juce::Colour(100, 100, 100));
		g.drawRect(0, 0, width, height, 1);
	}

	juce::Rectangle<int> getTooltipBounds(const juce::String& tipText, juce::Point<int> screenPos, juce::Rectangle<int> parentArea) override
	{
		juce::Rectangle<int> blankBounds(screenPos.x, screenPos.y, 0, 0);
		if (!settingsCache->toolTips) return blankBounds;

		juce::Font tooltipFont = juce::Font(14.0f);
		juce::TextLayout textLayout;
		int maxWidth = 200; // Maximum width of the tooltip

		juce::AttributedString attributedText;
		attributedText.setWordWrap(juce::AttributedString::WordWrap::byWord);
		attributedText.setJustification(juce::Justification::centred);
		attributedText.append(tipText, tooltipFont, juce::Colours::black);
		textLayout.createLayout(attributedText, static_cast<float>(maxWidth - 10)); // Subtract padding

		int width = juce::jmin(maxWidth, tooltipFont.getStringWidth(tipText) + 10);
		int height = static_cast<int>(textLayout.getHeight() + 10); // Add padding

		juce::Rectangle<int> bounds(screenPos.x, screenPos.y, width, height);

		// Ensure the tooltip is fully visible within the parent area
		if (bounds.getBottom() > parentArea.getBottom())
			bounds.setBottom(parentArea.getBottom());
		if (bounds.getRight() > parentArea.getRight())
			bounds.setRight(parentArea.getRight());

		return bounds;
	}

	void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
	{
		auto fontSize = juce::jmin(14.0f, button.getHeight() * 0.75f);
		auto tickWidth = fontSize * 1.1f;

		juce::Rectangle<float> tickBounds(4.0f, (button.getHeight() - tickWidth) * 0.5f, tickWidth, tickWidth);

		g.setColour(juce::Colour(15, 15, 15));
		g.fillRect(tickBounds); // Drawing the outline with specified color

		if (button.getToggleState())
		{
			g.setColour(button.findColour(juce::ToggleButton::tickColourId));
			auto tick = juce::Path();
			tick.startNewSubPath(tickBounds.getX() + tickBounds.getWidth() * 0.2f, tickBounds.getCentreY());
			tick.lineTo(tickBounds.getCentreX(), tickBounds.getBottom() - tickBounds.getHeight() * 0.2f);
			tick.lineTo(tickBounds.getRight() - tickBounds.getWidth() * 0.2f, tickBounds.getY() + tickBounds.getHeight() * 0.2f);
			g.strokePath(tick, juce::PathStrokeType(2.5f));
		}

		g.setColour(button.findColour(juce::ToggleButton::textColourId));
		g.setFont(fontSize);

		g.setColour(juce::Colour(40, 40, 40));
		g.drawRect(tickBounds, 1.0f);

		if (!button.isEnabled())
			g.setOpacity(0.5f);

		auto textBounds = button.getLocalBounds().toFloat().withLeft(tickBounds.getRight() + 10.0f).withRight(button.getWidth() - 2.0f);
		g.drawFittedText(button.getButtonText(), textBounds.toType<int>(), juce::Justification::centredLeft, 10);
	}

	void drawButtonBackground(juce::Graphics& g, 
							  juce::Button& button, 
							  const juce::Colour& backgroundColour,
							  bool isButtonActive, 
							  bool isButtonDown) override
	{
		auto buttonArea = button.getLocalBounds();
		g.setColour(backgroundColour);

		g.fillRect(buttonArea);

		if (isButtonDown || isButtonActive)
		{
			g.setColour(button.findColour(juce::TextButton::buttonOnColourId));
			g.fillRect(buttonArea);
		}
	}

	void drawButtonText(juce::Graphics& g, 
						juce::TextButton& button, 
						bool shouldDrawButtonAsHighlighted, 
						bool shouldDrawButtonAsDown) override
	{
		
		g.setColour(button.findColour(juce::Label::textColourId));
		g.drawFittedText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred , 1);
	}

	void drawComboBoxTextWhenNothingSelected(juce::Graphics& g, 
											 juce::ComboBox& comboBox, 
											 juce::Label& label) override
	{
		g.setColour(comboBox.findColour(juce::ComboBox::textColourId));
		
	}
	
	juce::Label* createComboBoxTextBox(juce::ComboBox& comboBox) override
	{
		juce::Label* label = new juce::Label(juce::String(), juce::String());
		label->setInterceptsMouseClicks(false, false);
		return label;
	}

	void positionComboBoxText(juce::ComboBox& comboBox, 
							  juce::Label& label) override
	{
		if (comboBox.getComponentID() == "StemType") label.setBounds(0, 0, comboBox.getWidth(), comboBox.getHeight());
		else if (comboBox.getComponentID() == "Key") label.setBounds(0, 0, comboBox.getWidth(), comboBox.getHeight());
		else if (comboBox.getComponentID() == "BasslineLoop") label.setBounds(0, 0, comboBox.getWidth(), comboBox.getHeight());
		else if (comboBox.getComponentID() == "PreviewBass") label.setBounds(0, 0, comboBox.getWidth(), comboBox.getHeight());
		else throw std::exception("Combo box ID not handled.");
		label.setJustificationType(juce::Justification::centred);
		label.setFont(16.0f);

	}

	void drawComboBox(juce::Graphics& g, 
					  int width, 
					  int height,
					  bool isButtonDown, 
					  int buttonX, 
					  int buttonY, 
					  int buttomW, 
					  int buttonH, 
					  juce::ComboBox& comboBox) override 
	{
		auto comboArea = comboBox.getLocalBounds();
		g.setColour(comboBox.findColour(juce::ComboBox::backgroundColourId));

		// create rounded rectangle
		juce::Path p;
		if (comboBox.getComponentID() != "BasslineLoop" && comboBox.getComponentID() != "PreviewBass")
		{
			
			p.addRoundedRectangle(comboArea, 5.0f);

			g.fillPath(p);
		}
		else
		{
			g.fillRect(0, 0, width, height);
		}
		
		if (comboBox.getComponentID() == "BasslineLoop" || comboBox.getComponentID() == "PreviewBass")
		{
			g.setColour(juce::Colour(40, 40, 40));
			g.drawRect(0, 0, width, height);

			if (isButtonDown || comboBox.isPopupActive() || comboBox.isMouseOver())
			{
				g.setColour(juce::Colour(30, 30, 30));
				g.fillRect(0, 0, width, height);
			}

		}
		else if (isButtonDown || comboBox.isPopupActive() || comboBox.isMouseOver()) {
			g.setColour(juce::Colour(98, 0, 255));
			g.fillPath(p);
		}

		
		
	}

	void drawPopupMenuBackground(juce::Graphics& g, 
								 int width, 
								 int height) override
	{
		g.setColour(juce::Colour(20, 20, 20));
		g.fillRect(0, 0, width, height);
	}

	void drawPopupMenuItem(juce::Graphics& g, 
						   const juce::Rectangle< int >& area, 
						   bool isSeparator, 
						   bool isActive, 
						   bool isHighlighted, 
						   bool isTicked, 
						   bool hasSubMenu, 
						   const juce::String& text, 
						   const juce::String& shortcutKeyText, 
						   const juce::Drawable* icon, 
						   const juce::Colour* textColour) override
	{
		g.setColour(juce::Colour(20, 20, 20));
		g.fillRect(area);

		if (text != "4 Bars" && text != "8 Bars" && text != "2 Bars") g.setFont(juce::Font(16.0f));

		if (isHighlighted || isTicked) 
		{
			g.setColour(juce::Colour(30,30,30));
			g.fillRect(area.reduced(2, 2));
		}
		
		g.setColour(juce::Colour(150,150,150));
		g.drawText(text, area.reduced(10, 0), juce::Justification::centred);
		
	}

	void getIdealPopupMenuItemSize(const juce::String& text, 
								   const bool isSeparator, 
								   int standardMenuItemHeight, 
								   int& idealWidth, 
								   int& idealHeight) override
	{
		idealWidth = 10;
		idealHeight = 25;
	}

	void drawLinearSlider(juce::Graphics& g,
						  int	x,
						  int	y,
						  int	width,
						  int	height,
						  float	sliderPos,
						  float	minSliderPos,
						  float	maxSliderPos,
						  juce::Slider::SliderStyle style,
						  juce::Slider& slider)
	{
		g.setColour(slider.findColour(juce::Slider::textBoxHighlightColourId));

		// Define the radius for the rounded corners
		float cornerRadius = 3.0f;  // Adjust this value to control the roundness of the corners

		// Draw the filled rectangle with rounded corners
		juce::Rectangle<float> rect((float)x, (float)y, sliderPos - (float)x, (float)height);
		g.fillRoundedRectangle(rect, cornerRadius);
		//slider.out
		

	}



private:
	juce::Font synergyFont;
	SettingsCache* settingsCache;
};

