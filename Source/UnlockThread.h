#pragma once

#include <JuceHeader.h>
#include "ProductUnlock.h"

using namespace juce;

class UnlockThread : public Thread
{
public:
	UnlockThread(ProductUnlock* status) : Thread("Unlock Thread"), productUnlockStatus(status) {}

	void run() override
	{
		unlockData = productUnlockStatus->attemptWebserverUnlock("client@rumorvst.ai", licenseKey);
		
	}

	OnlineUnlockStatus::UnlockResult unlockData;
	String licenseKey;
	

private:
	ProductUnlock* productUnlockStatus;
	
};

