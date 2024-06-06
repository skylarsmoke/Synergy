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
    // TODO: implement minor scales

    // map of musical keys to their corresponding major scale notes
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

std::vector<MidiNote> BassGenerator::generateBassline(const MidiFile& inputMidiFile, const String& stemType, const std::string& inputMusicalKey)
{
    std::vector<MidiNote> newBassline;
    musicalKey = inputMusicalKey;

    if (stemType == "Melody")
    {
        // we first extract the midi notes
        std::vector<MidiNote> melodyNotes = extractMidiNotes(inputMidiFile);

        // then we generate a bassline from the melody notes
        newBassline = generateBasslineFromMelody(melodyNotes);
    }

   
    return newBassline;

}

std::vector<MidiNote> BassGenerator::generateBasslineFromMelody(const std::vector<MidiNote>& melodyNotes)
{
    std::vector<MidiNote> basslineNotes;
    float totalBeats = 0.0f;
    float barLength = 4.0f; // Assuming 4 beats per bar
    float maxBeats = 8 * barLength; // 8 bars

    for (const auto& note : melodyNotes)
    {
        if (totalBeats >= maxBeats)
            break;

        // Generate bass note an octave lower
        int bassPitch = note.pitch - 12;
        bassPitch = std::max(0, std::min(127, bassPitch));
        float noteLength = std::min(note.lengthInBeats, 1.0f); // Restricting note length to 1 beat

        if (isNoteInScale(bassPitch))
        {
            basslineNotes.push_back({ bassPitch, bassPitch / 12, totalBeats, noteLength });
        }
        else
        {
            // Adjust note to be in the scale if it's not
            MidiNote bassNote = getNextNoteFromChain(bassPitch);
            bassNote.startBeat = totalBeats;
            bassNote.lengthInBeats = noteLength;
            basslineNotes.push_back(bassNote);
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