#include "Theme.h"

/*
* Contains plugin theme information 
*/

Theme::Theme() {
	
	// TODO: Retrieve saved theme

	this->setTheme(1);
}

void Theme::setTheme(int themeID) {
	this->themeID = themeID;
	
	// set theme colors
	switch (themeID) {
	case 1:
		// default theme
		mainTextColor = Colour(150, 150, 150);
		mainButtonColor = Colour(20, 20, 20);
		mainSliderColor = Colour(0, 195, 255);
		break;
	case 2:
		// black and white theme

		break;
	}

	// TODO: Need to add logic to store the theme using SQLite
}
