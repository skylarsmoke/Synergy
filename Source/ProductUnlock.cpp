/*
  ==============================================================================

    Unlock.cpp
    Created: 19 May 2024 5:31:32pm
    Author:  skyla

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ProductUnlock.h"

//==============================================================================
ProductUnlock::ProductUnlock()
{
 
}

ProductUnlock::~ProductUnlock()
{

}


String ProductUnlock::getProductID()
{
    return "SynergyBass";
}

bool ProductUnlock::doesProductIDMatch(const String& serverID)
{
    return getProductID() == serverID;
}

String ProductUnlock::getWebsiteName()
{
    return "rumorvst.ai";
}

URL ProductUnlock::getServerAuthenticationURL()
{
    return URL("https://synergybassserver-m7kdgwaz6q-uw.a.run.app");
}

RSAKey ProductUnlock::getPublicKey()
{
    return RSAKey("11,602815978d207ee7ce4982c23d5c39729da90af57b850863165936256e3b7227");
}

String ProductUnlock::readReplyFromWebserver(const juce::String& email, const juce::String& licenseKey)
{
    juce::URL url(getServerAuthenticationURL()
                  .withParameter("product", getProductID())
                  .withParameter("email", email)
                  .withParameter("lk", licenseKey)
                  .withParameter("os", juce::SystemStats::getOperatingSystemName())
                  .withParameter("mach", getLocalMachineIDs()[0]));

    DBG("Trying to unlock via URL: " << url.toString(true));

    {
        juce::ScopedLock lock(streamCreationLock);
        stream.reset(new juce::WebInputStream(url, true));
    }
       
    if (stream->connect(nullptr))
    {
        auto* thread = juce::Thread::getCurrentThread();

        if (thread->threadShouldExit() || stream->isError())
            return {};

        auto contentLength = stream->getTotalLength();
        auto downloaded = 0;

        const size_t bufferSize = 0x8000;
        juce::HeapBlock<char> buffer(bufferSize);

        while (!(stream->isExhausted() || stream->isError() || thread->threadShouldExit()))
        {
            auto max = juce::jmin((int)bufferSize, contentLength < 0 ? std::numeric_limits<int>::max()
                                  : static_cast<int> (contentLength - downloaded));

            auto actualBytesRead = stream->read(buffer.get() + downloaded, max - downloaded);

            if (actualBytesRead < 0 || thread->threadShouldExit() || stream->isError())
                break;

            downloaded += actualBytesRead;

            if (downloaded == contentLength)
                break;
        }

        if (thread->threadShouldExit() || stream->isError() || (contentLength > 0 && downloaded < contentLength))
            return {};

        return { juce::CharPointer_UTF8(buffer.get()) };
    }

    return {};
}

void ProductUnlock::userCancelled() 
{
    juce::ScopedLock lock(streamCreationLock);

    if (stream != nullptr)
        stream->cancel();
}