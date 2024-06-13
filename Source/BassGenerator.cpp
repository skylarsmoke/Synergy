/*
  ==============================================================================

    BassGenerator.cpp
    Created: 25 May 2024 11:54:04pm
    Author:  skyla

  ==============================================================================
*/

#include "BassGenerator.h"

//==============================================================================
// utilizing Markov Chain for generative AI

BassGenerator::BassGenerator()
{
    std::srand(std::time(0)); // Seed for randomness
    initializeScales();
    loadData();
}

void BassGenerator::initializeScales()
{
    std::vector<int> majorScale = { 0, 2, 4, 5, 7, 9, 11 };
    std::vector<int> minorScale = { 0, 2, 3, 5, 7, 8, 10 };

    // map of musical keys to their corresponding major scale notes
    // Major Scales
    scales["C"] = majorScale;
    scales["C#"] = adjustScale(majorScale, 1);
    scales["D"] = adjustScale(majorScale, 2);
    scales["D#"] = adjustScale(majorScale, 3);
    scales["E"] = adjustScale(majorScale, 4);
    scales["F"] = adjustScale(majorScale, 5);
    scales["F#"] = adjustScale(majorScale, 6);
    scales["G"] = adjustScale(majorScale, 7);
    scales["G#"] = adjustScale(majorScale, 8);
    scales["A"] = adjustScale(majorScale, 9);
    scales["A#"] = adjustScale(majorScale, 10);
    scales["B"] = adjustScale(majorScale, 11);

    // Minor scales
    scales["Cm"] = minorScale;
    scales["C#m"] = adjustScale(minorScale, 1);
    scales["Dm"] = adjustScale(minorScale, 2);
    scales["D#m"] = adjustScale(minorScale, 3);
    scales["Em"] = adjustScale(minorScale, 4);
    scales["Fm"] = adjustScale(minorScale, 5);
    scales["F#m"] = adjustScale(minorScale, 6);
    scales["Gm"] = adjustScale(minorScale, 7);
    scales["G#m"] = adjustScale(minorScale, 8);
    scales["Am"] = adjustScale(minorScale, 9);
    scales["Bbm"] = adjustScale(minorScale, 10);
    scales["Bm"] = adjustScale(minorScale, 11);
}

std::vector<int> BassGenerator::adjustScale(const std::vector<int>& scale, int semiTones)
{
    std::vector<int> adjustedScale;

    for (int note : scale)
    {
        adjustedScale.push_back((note + semiTones) % 12);
    }

    return adjustedScale;
}

bool BassGenerator::isNoteInScale(int note)
{
    int noteInOctave = note % 12;
    const std::vector<int>& scale = scales[musicalKey];
    return std::find(scale.begin(), scale.end(), noteInOctave) != scale.end();
}

void BassGenerator::trainFromFolder(const String& folderPath)
{
    File folder(folderPath);

    if (!folder.isDirectory())
    {
        std::cerr << "Specified path is not a directory." << std::endl;
        return;
    }

    Array<File> midiFiles;
    folder.findChildFiles(midiFiles, File::findFiles, false, "*mid");

    // loop through all files in directory and call train
    for (const auto& midiFile : midiFiles)
    {
        File midiFile(midiFile.getFullPathName().toStdString());
        FileInputStream inputStream(midiFile);
        MidiFile juceMidiFile;
        juceMidiFile.readFrom(inputStream);

        train(juceMidiFile);
    }

    // TODO: Delete this before making production version
    //saveData("C:\\Users\\skyla\\OneDrive\\Desktop\\synergyBassData.json");
}

void BassGenerator::train(const MidiFile& midiFile)
{
    // extract notes and prepare training data
    std::vector<MidiNote> basslineNotes = extractMidiNotes(midiFile);

    // train the markov chain model
    trainChain(basslineNotes);
}

void BassGenerator::trainChain(const std::vector<MidiNote>& notes)
{
    for (size_t i = 0; i < notes.size() - 1; i++)
    {
        int currentNote = notes[i].pitch;
        MidiNote nextNote = notes[i + 1];
        markovChain[currentNote].push_back(nextNote);
    }
}

std::vector<MidiNote> BassGenerator::extractMidiNotes(const MidiFile& midiFile)
{
    std::vector<MidiNote> midiNotes;

    for (int i = 0; i < midiFile.getNumTracks(); i++)
    {
        auto* midiTrack = midiFile.getTrack(i);
        std::map<int, juce::MidiMessage> noteOnMap; // Map to store note-on events

        for (auto midiEvent : *midiTrack)
        {
            if (midiEvent->message.isNoteOn()) {
                int pitch = midiEvent->message.getNoteNumber();
                float startBeat = midiEvent->message.getTimeStamp();
                int octave = pitch / 12;
                noteOnMap[pitch] = midiEvent->message; // Store note-on event
            }
            else if (midiEvent->message.isNoteOff()) {
                int pitch = midiEvent->message.getNoteNumber();
                auto it = noteOnMap.find(pitch);
                if (it != noteOnMap.end()) {
                    float startBeat = it->second.getTimeStamp();
                    float lengthInBeats = midiEvent->message.getTimeStamp() - startBeat;
                    int octave = pitch / 12;
                    midiNotes.push_back({ pitch, octave, startBeat, lengthInBeats });
                    noteOnMap.erase(it); // Remove processed note-on event
                }
            }
        }
    }

    return midiNotes;
}

std::vector<MidiNote> BassGenerator::generateBassline(const MidiFile& inputMidiFile, 
                                                      const String& stemType, 
                                                      const std::string& inputMusicalKey,
                                                      int noteVariety,
                                                      int loopLength)
{
    std::vector<MidiNote> newBassline;

    // remove Major or Minor from key
    std::size_t spacePos = inputMusicalKey.find(' ');
    auto minorString = inputMusicalKey.find("Minor") != std::string::npos ? "m" : "";
    musicalKey = inputMusicalKey.substr(0, spacePos) + minorString;

    if (stemType == "Melody")
    {
        // we first extract the midi notes
        std::vector<MidiNote> melodyNotes = extractMidiNotes(inputMidiFile);

        // then we generate a bassline from the melody notes
        newBassline = generateBasslineFromMelody(melodyNotes, noteVariety, loopLength);
    }
    else if (stemType == "Chords")
    {
        // we first extract the midi notes
        std::vector<MidiNote> chordNotes = extractMidiNotes(inputMidiFile);

        // then we generate a bassline from the chord notes
        newBassline = generateBasslineFromChords(chordNotes, noteVariety, loopLength);
    }
    else if (stemType == "Drums")
    {
        // we first extract the midi notes
        std::vector<MidiNote> drumNotes = extractMidiNotes(inputMidiFile);

        // then we generate a bassline from the drum notes
        newBassline = generateBasslineFromDrums(drumNotes, noteVariety, loopLength);
    }

   
    return newBassline;

}

std::vector<MidiNote> BassGenerator::generateBasslineFromMelody(const std::vector<MidiNote>& melodyNotes,
                                                                int noteVariety,
                                                                int loopLength)
{
    std::vector<MidiNote> basslineNotes;
    std::vector<MidiNote> loopedBassline;
    float totalBeats = 0.0f;
    float barLength = 4.0f; // Assuming 4 beats per bar
    float maxBeats = 8 * barLength; // 8 bars
    float loopBeats = loopLength * barLength; // Length of the loop in beats

    if (melodyNotes.empty()) return basslineNotes;

    // Use the first note's pitch as the reference
    int referencePitch = melodyNotes[0].pitch;
    int referenceOctave = referencePitch / 12;

    while (totalBeats < maxBeats)
    {
        // If we're at the start of a loop, and we have a saved loop, append the saved loop
        if (totalBeats > 0 && static_cast<int>(totalBeats) % static_cast<int>(loopBeats) == 0 && !loopedBassline.empty())
        {
            for (const auto& note : loopedBassline)
            {
                MidiNote loopedNote = note;
                loopedNote.startBeat += totalBeats;
                basslineNotes.push_back(loopedNote);
            }
            totalBeats += loopBeats;
            continue;
        }

        // Select a note from the melody or generate a new one based on noteVariety
        MidiNote melodyNote = melodyNotes[std::rand() % melodyNotes.size()];

        // Determine if we should use the melody note or generate a new note based on noteVariety
        int useMelodyNote = std::rand() % 100;
        int bassPitch;
        if (useMelodyNote < (100 - noteVariety))
        {
            bassPitch = melodyNote.pitch;
        }
        else
        {
            bassPitch = getNextNoteFromChain(melodyNote.pitch).pitch;
        }

        // Constrain the pitch to within one octave of the reference pitch
        while (bassPitch < referencePitch - 12) bassPitch += 12;
        while (bassPitch > referencePitch + 12) bassPitch -= 12;

        bassPitch = std::max(0, std::min(127, bassPitch));

        // Ensure the pitch is within one octave of the reference note
        int bassOctave = bassPitch / 12;
        if (bassOctave != referenceOctave)
        {
            bassPitch = (bassPitch % 12) + (referenceOctave * 12);
        }

        // Randomize the note length between a quarter note (1 beat) and a whole note (4 beats)
        float noteLength = (std::rand() % 4 + 1) * 1.0f;

        // Ensure the note fits within the remaining beats
        noteLength = std::min(noteLength, maxBeats - totalBeats);

        // Limit the number of attempts to find a valid note to avoid infinite loops
        int attempts = 0;
        const int maxAttempts = 10;

        while (!isNoteInScale(bassPitch) && attempts < maxAttempts)
        {
            bassPitch = std::rand() % 128;
            // Constrain the pitch to within one octave of the reference pitch
            while (bassPitch < referencePitch - 12) bassPitch += 12;
            while (bassPitch > referencePitch + 12) bassPitch -= 12;
            attempts++;
        }

        if (isNoteInScale(bassPitch))
        {
            MidiNote newNote = { bassPitch, bassPitch / 12, totalBeats, noteLength };
            basslineNotes.push_back(newNote);
            loopedBassline.push_back(newNote);
        }
        else
        {
            // If no valid note was found, use the closest note in the scale
            bassPitch = scales[musicalKey][std::rand() % scales[musicalKey].size()];
            // Constrain the pitch to within one octave of the reference pitch
            while (bassPitch < referencePitch - 12) bassPitch += 12;
            while (bassPitch > referencePitch + 12) bassPitch -= 12;
            MidiNote newNote = { bassPitch, bassPitch / 12, totalBeats, noteLength };
            basslineNotes.push_back(newNote);
            loopedBassline.push_back(newNote);
        }

        totalBeats += noteLength;
    }

    return basslineNotes;

}

std::vector<MidiNote> BassGenerator::generateBasslineFromChords(const std::vector<MidiNote>& chordNotes,
                                                                int noteVariety,
                                                                int loopLength)
{
    std::vector<MidiNote> basslineNotes;
    std::vector<MidiNote> loopedBassline;
    float totalBeats = 0.0f;
    float barLength = 4.0f; // Assuming 4 beats per bar
    float maxBeats = 8 * barLength; // 8 bars
    float loopBeats = loopLength * barLength; // Length of the loop in beats

    if (chordNotes.empty()) return basslineNotes;

    // Group chord notes by start beat
    std::map<float, std::vector<MidiNote>> chordsByBeat;
    for (const auto& note : chordNotes)
    {
        chordsByBeat[note.startBeat].push_back(note);
    }

    // Use the first chord's root note as the reference
    int referencePitch = chordNotes[0].pitch;
    int referenceOctave = referencePitch / 12;

    while (totalBeats < maxBeats)
    {
        // If we're at the start of a loop, and we have a saved loop, append the saved loop
        if (totalBeats > 0 && static_cast<int>(totalBeats) % static_cast<int>(loopBeats) == 0 && !loopedBassline.empty())
        {
            for (const auto& note : loopedBassline)
            {
                MidiNote loopedNote = note;
                loopedNote.startBeat += totalBeats;
                basslineNotes.push_back(loopedNote);
            }
            totalBeats += loopBeats;
            continue;
        }

        // Get the chord for the current beat
        float currentBeat = std::fmod(totalBeats, loopBeats);
        auto it = chordsByBeat.lower_bound(currentBeat);
        if (it == chordsByBeat.end()) it = chordsByBeat.begin(); // Loop back if no chord is found

        const std::vector<MidiNote>& chord = it->second;

        // Select a note from the chord or generate a new one based on noteVariety
        MidiNote chordNote = chord[std::rand() % chord.size()];

        // Determine if we should use the chord note or generate a new note based on noteVariety
        int useChordNote = std::rand() % 100;
        int bassPitch;
        if (useChordNote < (100 - noteVariety))
        {
            bassPitch = chordNote.pitch;
        }
        else
        {
            bassPitch = getNextNoteFromChain(chordNote.pitch).pitch;
        }

        // Constrain the pitch to within one octave of the reference pitch
        while (bassPitch < referencePitch - 12) bassPitch += 12;
        while (bassPitch > referencePitch + 12) bassPitch -= 12;

        bassPitch = std::max(0, std::min(127, bassPitch));

        // Ensure the pitch is within one octave of the reference note
        int bassOctave = bassPitch / 12;
        if (bassOctave != referenceOctave)
        {
            bassPitch = (bassPitch % 12) + (referenceOctave * 12);
        }

        // Randomize the note length between a quarter note (1 beat) and a whole note (4 beats)
        float noteLength = (std::rand() % 4 + 1) * 1.0f;

        // Ensure the note fits within the remaining beats
        noteLength = std::min(noteLength, maxBeats - totalBeats);

        // Limit the number of attempts to find a valid note to avoid infinite loops
        int attempts = 0;
        const int maxAttempts = 10;

        while (!isNoteInScale(bassPitch) && attempts < maxAttempts)
        {
            bassPitch = std::rand() % 128;
            // Constrain the pitch to within one octave of the reference pitch
            while (bassPitch < referencePitch - 12) bassPitch += 12;
            while (bassPitch > referencePitch + 12) bassPitch -= 12;
            attempts++;
        }

        if (isNoteInScale(bassPitch))
        {
            MidiNote newNote = { bassPitch, bassPitch / 12, totalBeats, noteLength };
            basslineNotes.push_back(newNote);
            loopedBassline.push_back(newNote);
        }
        else
        {
            // If no valid note was found, use the closest note in the scale
            bassPitch = scales[musicalKey][std::rand() % scales[musicalKey].size()];
            // Constrain the pitch to within one octave of the reference pitch
            while (bassPitch < referencePitch - 12) bassPitch += 12;
            while (bassPitch > referencePitch + 12) bassPitch -= 12;
            MidiNote newNote = { bassPitch, bassPitch / 12, totalBeats, noteLength };
            basslineNotes.push_back(newNote);
            loopedBassline.push_back(newNote);
        }

        totalBeats += noteLength;
    }

    return basslineNotes;
}

std::vector<MidiNote> BassGenerator::generateBasslineFromDrums(const std::vector<MidiNote>& drumNotes,
                                                               int noteVariety,
                                                               int loopLength)
{
    std::vector<MidiNote> basslineNotes;
    std::vector<MidiNote> loopedBassline;
    float totalBeats = 0.0f;
    float barLength = 4.0f; // Assuming 4 beats per bar
    float maxBeats = 8 * barLength; // 8 bars
    float loopBeats = loopLength * barLength; // Length of the loop in beats

    if (drumNotes.empty()) return basslineNotes;

    // Filter out kick drum notes (assuming MIDI note number 36 for kick)
    std::vector<MidiNote> kickNotes;
    for (const auto& note : drumNotes)
    {
        if (note.pitch == 36) // MIDI note number for kick drum
        {
            kickNotes.push_back(note);
        }
    }

    if (kickNotes.empty()) return basslineNotes;

    // Extract beat markers from kick notes
    std::vector<float> beatMarkers;
    for (const auto& kickNote : kickNotes)
    {
        beatMarkers.push_back(kickNote.startBeat);
    }

    // Use the first kick note's startBeat as the reference
    float referenceBeat = kickNotes[0].startBeat;

    while (totalBeats < maxBeats)
    {
        // If we're at the start of a loop, and we have a saved loop, append the saved loop
        if (totalBeats > 0 && static_cast<int>(totalBeats) % static_cast<int>(loopBeats) == 0 && !loopedBassline.empty())
        {
            for (const auto& note : loopedBassline)
            {
                MidiNote loopedNote = note;
                loopedNote.startBeat += loopBeats;
                basslineNotes.push_back(loopedNote);
            }
            totalBeats += loopBeats;
            continue;
        }

        // Select a beat marker for the current beat
        float currentBeat = std::fmod(totalBeats, loopBeats);
        auto it = std::lower_bound(beatMarkers.begin(), beatMarkers.end(), currentBeat);
        if (it == beatMarkers.end()) it = beatMarkers.begin(); // Loop back if no beat marker is found

        float beatMarker = *it;

        // Determine if we should use a beat marker or generate a new one based on noteVariety
        int useBeatMarker = std::rand() % 100;
        if (useBeatMarker >= noteVariety)
        {
            beatMarker += (std::rand() % 4 + 1) * 0.25f; // Randomly shift the beat marker within a quarter note
            beatMarker = std::fmod(beatMarker, loopBeats); // Ensure it stays within the loop length
        }

        // Generate a bass note based on the beat marker
        int bassPitch = getNextNoteFromChain(static_cast<int>(referenceBeat)).pitch;

        // Constrain the pitch to within one octave of a reference note (e.g., C2, MIDI note number 36)
        int referencePitch = 36; // C2
        while (bassPitch < referencePitch - 12) bassPitch += 12;
        while (bassPitch > referencePitch + 12) bassPitch -= 12;

        bassPitch = std::max(0, std::min(127, bassPitch));

        // Randomize the note length between a quarter note (1 beat) and a whole note (4 beats)
        float noteLength = (std::rand() % 4 + 1) * 1.0f;

        // Ensure the note fits within the remaining beats
        noteLength = std::min(noteLength, loopBeats - currentBeat);

        // Limit the number of attempts to find a valid note to avoid infinite loops
        int attempts = 0;
        const int maxAttempts = 10;

        while (!isNoteInScale(bassPitch) && attempts < maxAttempts)
        {
            bassPitch = std::rand() % 128;
            // Constrain the pitch to within one octave of the reference pitch
            while (bassPitch < referencePitch - 12) bassPitch += 12;
            while (bassPitch > referencePitch + 12) bassPitch -= 12;
            attempts++;
        }

        if (isNoteInScale(bassPitch))
        {
            MidiNote newNote = { bassPitch, bassPitch / 12, totalBeats, noteLength };
            basslineNotes.push_back(newNote);
            loopedBassline.push_back(newNote);
        }
        else
        {
            // If no valid note was found, use the closest note in the scale
            bassPitch = scales[musicalKey][std::rand() % scales[musicalKey].size()];
            // Constrain the pitch to within one octave of the reference pitch
            while (bassPitch < referencePitch - 12) bassPitch += 12;
            while (bassPitch > referencePitch + 12) bassPitch -= 12;
            MidiNote newNote = { bassPitch, bassPitch / 12, totalBeats, noteLength };
            basslineNotes.push_back(newNote);
            loopedBassline.push_back(newNote);
        }

        totalBeats += noteLength;
    }

    return basslineNotes;
}

MidiNote BassGenerator::getNextNoteFromChain(int currentNote)
{
    if (markovChain.find(currentNote) != markovChain.end())
    {
        const std::vector<MidiNote>& possibleNextNotes = markovChain[currentNote];
        int randomIndex = std::rand() % possibleNextNotes.size();
        return possibleNextNotes[randomIndex];
    }
    else
    {
        // if no next note is found return a random note
        int note;
        do {
            note = std::rand() % 128;
        } while (!isNoteInScale(note));
        return { note, note / 12, 0.0f, 1.0f };
    }
}

void BassGenerator::saveData(const std::string& pathToOutputData)
{
    // create JSON from data
    DynamicObject* jsonData = new DynamicObject();

    for (const auto& [key, value] : markovChain)
    {
        Array<var> jsonArray;
        for (const auto& midiNote : value)
        {
            DynamicObject* noteData = new DynamicObject();
            noteData->setProperty("pitch", midiNote.pitch);
            noteData->setProperty("octave", midiNote.octave);
            noteData->setProperty("startBeat", midiNote.startBeat);
            noteData->setProperty("lengthInBeats", midiNote.lengthInBeats);
            jsonArray.add(juce::var(noteData));
        }
        jsonData->setProperty(String(key), jsonArray);
    }

    // create file
    File file(pathToOutputData);
    FileOutputStream outputStream(file);

    if (outputStream.openedOk())
    {
        var jsonVar(jsonData);
        String jsonString = JSON::toString(jsonVar, true);
        outputStream.writeText(jsonString, false, false, "\n");
        outputStream.flush();
    }
    else
    {
        std::cerr << "Unable to open file for writing: " << pathToOutputData << std::endl;
    }

}

void BassGenerator::loadData()
{
    auto inputData = MemoryInputStream(BinaryData::synergyBassData_json, BinaryData::synergyBassData_jsonSize, false);

    if (inputData.getTotalLength() != 0)
    {
        var jsonVar = JSON::parse(inputData);
        
        if (jsonVar.isObject())
        {
            DynamicObject* jsonData = jsonVar.getDynamicObject();
            markovChain.clear(); // we clear our current chain data as a precaution

            for (const auto& prop : jsonData->getProperties())
            {
                int key = prop.name.toString().getIntValue();
                const Array<var>& jsonArray = *prop.value.getArray();
                std::vector<MidiNote> values;

                for (const auto& v : jsonArray)
                {
                    DynamicObject* noteData = v.getDynamicObject();
                    MidiNote midiNote{
                        noteData->getProperty("pitch"),
                        noteData->getProperty("octave"),
                        noteData->getProperty("startBeat"),
                        noteData->getProperty("lengthInBeats")
                    };
                    values.push_back(midiNote);
                }

                markovChain[key] = values;
            }
        }
    }
    else
    {
        std::cerr << "Unable to open file for reading" << std::endl;
    }
}