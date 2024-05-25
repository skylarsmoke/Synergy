/*
  ==============================================================================

    ProductLockScreen.h
    Created: 19 May 2024 5:59:47pm
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ProductUnlock.h"
#include "UnlockThread.h"

using namespace juce;

//==============================================================================
/*
*/
class ProductLockScreen  : public juce::Component, Timer
{
public:
    ProductLockScreen(ProductUnlock* status, TextEditor& messageBox);
    ~ProductLockScreen() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void dismiss();
    void reactivate();

private:
    Label activationLabel;
    Label licenseKeyInput{};
    Label licenseKeyLabel;
    Label activationStatusLabel;

    TextEditor& messageBox;

    TextButton activateButton;
    HyperlinkButton havingTroubleActivatingLink;

    ProductUnlock* productUnlockStatus;
    UnlockThread unlockThread;

    bool activateAttempted = false;;

    void activate();

    /// <summary>
    /// Logs data in the message box
    /// </summary>
    /// <param name="m"></param>
    void logMessage(const juce::String& m) {
        messageBox.moveCaretToEnd();
        messageBox.insertTextAtCaret(m + juce::newLine);
    }

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProductLockScreen)
};
