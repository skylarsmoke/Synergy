/*
  ==============================================================================

    Unlock.h
    Created: 19 May 2024 5:31:32pm
    Author:  skyla

    This class contains the logic for authenticating and unlocking the plugin.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

//==============================================================================
/*
*/
class ProductUnlock : public OnlineUnlockStatus
{
public:
    ProductUnlock();
    ~ProductUnlock() override;

    String getProductID() override;
    bool doesProductIDMatch(const String& id) override;
    String getWebsiteName() override;
    URL getServerAuthenticationURL() override;
    RSAKey getPublicKey() override;
    void saveState(const String&) override {}
    String getState() override { return {}; }
    String readReplyFromWebserver(const juce::String& email, const juce::String& password) override;
    void userCancelled() override;

private:
    juce::CriticalSection streamCreationLock;
    std::unique_ptr<juce::WebInputStream> stream;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProductUnlock)
};
