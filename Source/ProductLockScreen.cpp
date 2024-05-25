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
ProductLockScreen::ProductLockScreen(ProductUnlock* status, TextEditor& messageBox) : messageBox(messageBox), unlockThread(status)
{
    startTimer(3000);
    productUnlockStatus = status;
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
    activateButton.onClick = [this] { activate(); };
    addAndMakeVisible(activateButton);

    // having trouble link
    havingTroubleActivatingLink.setButtonText("Having trouble?");
    havingTroubleActivatingLink.setColour(HyperlinkButton::textColourId, Colour(60, 60, 60));
    havingTroubleActivatingLink.setURL(URL("https://google.com"));
    addAndMakeVisible(havingTroubleActivatingLink);

    // activation status label
    activationStatusLabel.setColour(Label::textColourId, Colour(100, 100, 100));
    activationStatusLabel.setFont(13.8f);
    activationStatusLabel.setJustificationType(Justification::centred);
    addChildComponent(activationStatusLabel);

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

    // activation status
    activationStatusLabel.setBounds(300, 410, 300, 20);

}

void ProductLockScreen::timerCallback()
{
    if (!unlockThread.unlockData.succeeded)
    {
        if (unlockThread.unlockData.errorMessage == "Invalid License Key")
        {
            activationStatusLabel.setText("Invalid License Key", NotificationType::dontSendNotification);
        }
        else if (unlockThread.unlockData.errorMessage == "This license key is registered to a different machine.")
        {
            activationStatusLabel.setText("This license key is registered to a different machine.", NotificationType::dontSendNotification);
        }
        else
        {
            activationStatusLabel.setText("Unable to activate. Please check your connection.", NotificationType::dontSendNotification);
        }
        
        activateAttempted = false;
    }
    else if (unlockThread.unlockData.succeeded && productUnlockStatus->isUnlocked()) {
        activationStatusLabel.setText("Activation successful.", NotificationType::dontSendNotification);
        logMessage("Product activated.");

        stopTimer();
        activateAttempted = false;

        File licenseFile(File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile).getParentDirectory().getChildFile("sbr_license.txt"));
        licenseFile.replaceWithText(licenseKeyInput.getText());
    }
}

void ProductLockScreen::dismiss() 
{
    setVisible(false);
}

void ProductLockScreen::activate()
{
    logMessage("Activation initialized.");

    // activation status shown to user
    activationStatusLabel.setText("Sending request to server...", NotificationType::dontSendNotification);
    activationStatusLabel.setVisible(true);
    activateAttempted = true;
    unlockThread.licenseKey = licenseKeyInput.getText();
    unlockThread.startThread();

    while (!unlockThread.waitForThreadToExit(0));

    auto x = 0;
}

void ProductLockScreen::reactivate()
{
    // cached license key
    File licenseFile = File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile).getParentDirectory().getChildFile("sbr_license.txt");

    // if the license file exists we attempt to verify
    if (licenseFile.existsAsFile())
    {
        std::unique_ptr<FileInputStream> inputLicenseFile(licenseFile.createInputStream());

        // if the file was opened
        if (inputLicenseFile->openedOk())
        {
            const String licenseKey = inputLicenseFile->readString();
            licenseKeyInput.setText(licenseKey, NotificationType::dontSendNotification);
            activate();
        }
    }
}

