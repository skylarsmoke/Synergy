/*
  ==============================================================================

    ProductLockScreen.cpp
    Created: 19 May 2024 5:59:47pm
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ProductLockScreen.h"

//==============================================================================
ProductLockScreen::ProductLockScreen()
{
    setSize(900, 700);

    // activation label
    activationLabel.setText("Product Activation", NotificationType::dontSendNotification);
    activationLabel.setFont(20.0f);
    activationLabel.setColour(Label::textColourId, Colour(120, 120, 120));
    addAndMakeVisible(activationLabel);

    // license key label
    licenseKeyLabel.setText("License Key:", NotificationType::dontSendNotification);
    licenseKeyLabel.attachToComponent(&licenseKeyInput, true);
    licenseKeyLabel.setColour(Label::textColourId, Colour(100, 100, 100));
    licenseKeyLabel.setFont(13.8f);
    addAndMakeVisible(licenseKeyLabel);

    // license key input
    licenseKeyInput.setEditable(true);
    licenseKeyInput.setColour(Label::backgroundColourId, Colour(15, 15, 15));
    licenseKeyInput.setColour(Label::textColourId, Colour(100, 100, 100));
    licenseKeyInput.setColour(Label::textWhenEditingColourId, Colour(100, 100, 100));
    licenseKeyInput.setColour(Label::outlineWhenEditingColourId, Colour(15, 15, 15));
    licenseKeyInput.setFont(13.8f);
    addAndMakeVisible(licenseKeyInput);

    // activate button
    activateButton.setButtonText("Activate");
    activateButton.setColour(TextButton::buttonColourId, Colour(15, 15, 15));
    activateButton.setColour(TextButton::textColourOffId, Colour(100, 100, 100));
    activateButton.setColour(TextButton::textColourOnId, Colour(100, 100, 100));
    activateButton.setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(activateButton);

    // having trouble link
    havingTroubleActivatingLink.setButtonText("Having trouble?");
    havingTroubleActivatingLink.setColour(HyperlinkButton::textColourId, Colour(60, 60, 60));
    havingTroubleActivatingLink.setURL(URL("https://google.com"));
    addAndMakeVisible(havingTroubleActivatingLink);
}

ProductLockScreen::~ProductLockScreen()
{
}

void ProductLockScreen::paint (juce::Graphics& g)
{
    
    g.fillAll (Colour((uint8)0,0,0,0.6f));  // shaded background

    g.setColour(Colour(25, 25, 25));
    g.fillRect(250, 190, 400, 270);

    g.setColour(Colour(100, 100, 100));
    g.drawFittedText("Please enter the license key you received by email. This email is the one you used when purchasing Synergy Bass.",
                     272, 260, 398, 50, Justification::left, 3);
    
}

void ProductLockScreen::resized()
{
    // activation label
    activationLabel.setBounds(372, 230, 200, 20);

    // license key
    licenseKeyInput.setBounds(357, 320, 275, 20);

    // activate button
    activateButton.setBounds(400, 370, 100, 30);

    // having trouble link
    havingTroubleActivatingLink.setBounds(396, 435, 110, 20);

}
