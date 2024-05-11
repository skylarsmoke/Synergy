#pragma once

/*
* Contains all data for each musical key
*/

#include <map>
#include <vector>
#include <iostream>

using namespace std;

map<string, int> noteToNoteNumber = { 
	{ "C", 48 },
	{ "C#", 49 }, // Db
	{ "D", 50 },
	{ "D#", 51 }, // Eb
	{ "E", 52 },
	{ "F", 53 },
	{ "F#", 54 }, // Gb
	{ "G", 55 },
	{ "G#", 56 }, // Ab
	{ "A", 57 },
	{ "A#", 58 }, // Bb
	{ "B", 59 }
};

// TODO: Add functionality for other keys besides major and minor
map<string, vector<string>> notePerKey = {
	{ "Ab Major", { "Ab" , "Bb", "C", "Db", "Eb", "F", "G" }},
	{ "A Major", {  "A" , "B", "C#", "D", "E", "F#" , "G#" }},
	{ "Bb Major", { "Bb" , "C" , "D" , "Eb" , "F" , "G" , "A"  }},
	{ "B Major", {"B" ,  "C#" ,"D#" , "E" , "F#" , "G#" , "A#" }},
	{ "C Major", {"C" ,  "D" , "E" ,  "F" , "G" , "A" , "B"  }},
	{ "Db Major", {"Db" , "Eb" , "F" , "Gb" , "Ab" , "Bb" , "C" }},
	{ "D Major", { "D" ,  "E" , "F#" , "G" , "A" ,  "B" , "C#" }},
	{ "Eb Major", { "Eb" , "F" , "G", "Ab", "Bb", "C" ,  "D" }},
	{ "E Major", { "E" , "F#" ,  "G#" ,  "A" , "B" , "C#" , "D#"  }},
	{ "F Major", { "F" , "G" , "A" ,  "Bb" , "C" , "D" , "E" }},
	{ "F# Major", { "F#" , "G#" , "A#" ,  "B" , "C#" , "D#" , "E#"  }},
	{ "G Major", { "G" , "A" , "B" , "C" , "D" , "E" , "F#"  }},
	{ "A Minor", { "A" ,  "B" , "C" , "D" , "E" ,  "F" , "G" }},
	{ "Bb Minor", { "Bb" ,  "C" ,  "Db" , "Eb" , "F" ,  "Gb" , "Ab"  }},
	{ "B Minor", { "B" , "C#" ,  "D" ,  "E" , "F#" , "G" ,"A"  }},
	{ "C Minor", { "C" ,  "D" ,  "Eb" , "F" ,  "G" , "Ab" , "Bb"  }},
	{ "C# Minor", { "C#" ,  "D#" ,  "E" ,"F#" ,  "G#" ,  "A" ,  "B"  }},
	{ "D Minor", { "D" ,  "E" , "F" ,  "G" , "A" ,  "Bb" , "C"  }},
	{ "D# Minor", { "D#" ,  "E#" ,  "F#" ,  "G#" , "A#" ,  "B" , "C#"  }},
	{ "E Minor", { "E" ,  "F#" ,  "G" ,  "A" , "B" , "C" , "D"  }},
	{ "F Minor", { "F" ,  "G" ,  "Ab" ,  "Bb" , "C" ,  "Db" ,  "Eb"  }},
	{ "F# Minor", {"F#" ,  "G#" , "A" ,  "B" , "C#" , "D" , "E" }},
	{ "G Minor", { "G" ,  "A" ,  "Bb" ,  "C" ,  "D" , "Eb" , "F"  }},
	{ "G# Minor", { "G#" ,  "A#" ,  "B" ,  "C#" , "D#" ,  "E" ,  "F#"  }}
	
};

