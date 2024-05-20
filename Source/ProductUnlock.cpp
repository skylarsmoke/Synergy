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
    return URL("https:://localhost:8443/auth.php");
}

RSAKey ProductUnlock::getPublicKey()
{
    return RSAKey("INSERT_PUBLIC_KEY_HERE");
}
