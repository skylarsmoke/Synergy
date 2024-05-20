/*
  ==============================================================================

    ProductLockScreen.h
    Created: 19 May 2024 5:59:47pm
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

//==============================================================================
/*
*/
class ProductLockScreen  : public juce::Component
{
public:
    ProductLockScreen();
    ~ProductLockScreen() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Label activationLabel;
    Label licenseKeyInput;
    Label licenseKeyLabel;

    TextButton activateButton;
    HyperlinkButton havingTroubleActivatingLink;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProductLockScreen)
};
