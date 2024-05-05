#include "Theme.h"

/*
* Contains plugin theme information 
*/

Theme::Theme() {
	
	// TODO: Retrieve saved theme

	themeID = 1;
}

int Theme::getThemeID() {
	return this->themeID;
}

void Theme::setTheme(int themeID) {
	this->themeID = themeID;

	// TODO: Need to add logic to store the theme using SQLite
}