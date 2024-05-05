#pragma once

/*
* Contains logic for the plugin theme
*/

#include <map>
#include <iostream>

using namespace std;

class Theme
{
public:
	Theme();

	// variables
	int themeID;
	map<int, string> themes = { {1, "Default"}, {2, "BlackAndWhite"}};

	/// <summary>
	/// Gets the current theme ID
	/// </summary>
	/// <returns></returns>
	int getThemeID();

	/// <summary>
	/// Sets the plugin theme from a theme ID
	/// </summary>
	/// <param name="themeID">Theme ID</param>
	void setTheme(int themeID);

};

