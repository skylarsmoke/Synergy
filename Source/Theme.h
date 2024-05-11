#pragma once

/*
* Contains logic for the plugin theme
*/

#include <map>
#include <iostream>
#include <JuceHeader.h>


using namespace std;
using namespace juce;

class Theme
{
public:
	Theme();

	// variables
	int themeID;
	map<int, string> themes = { {1, "Default"}, {2, "BlackAndWhite"}};
	Colour mainTextColor;
	Colour mainButtonColor;

	/// <summary>
	/// Sets the plugin theme from a theme ID
	/// </summary>
	/// <param name="themeID">Theme ID</param>
	void setTheme(int themeID);

};

