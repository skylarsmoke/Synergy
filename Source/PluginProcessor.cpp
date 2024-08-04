/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynergyAudioProcessor::SynergyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    parameters(*this, nullptr, "PARAMETERS",
               { std::make_unique<juce::AudioParameterFloat>("variety", "Variety", 0.0f, 1.0f, 0.5f),
                 std::make_unique<juce::AudioParameterFloat>("param2", "Parameter 2", 0.0f, 1.0f, 0.5f) })
#endif
{
    formatManager.registerBasicFormats();

    for (int i = 0; i < 4; i++)
    {
        synth.addVoice(new juce::SamplerVoice());
    }

    std::unique_ptr<juce::AudioFormatReader> audioFormatReader(
        formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(
            BinaryData::Outta_Space_wav, BinaryData::Outta_Space_wavSize, false
        ))
    );

    if (audioFormatReader != nullptr)
    {
        juce::BigInteger allNotes;
        allNotes.setRange(0, 128, true);
        synth.addSound(new juce::SamplerSound("OuttaSpace", *audioFormatReader,
                                              allNotes, 36, 0.1, 0.1, 5.0));
    } 

    /*synth.addVoice(new Bass1Voice());
    synth.addSound(new Bass1());*/


}

SynergyAudioProcessor::~SynergyAudioProcessor()
{
   
}

//==============================================================================
const juce::String SynergyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynergyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynergyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynergyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynergyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynergyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynergyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynergyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynergyAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynergyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynergyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    this->sampleRate = sampleRate;
    currentPosition = 0.0;
}

void SynergyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynergyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynergyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //juce::ScopedNoDenormals noDenormals;
    //auto totalNumInputChannels  = getTotalNumInputChannels();
    //auto totalNumOutputChannels = getTotalNumOutputChannels();

    //// In case we have more outputs than inputs, this code clears any output
    //// channels that didn't contain input data, (because these aren't
    //// guaranteed to be empty - they may contain garbage).
    //// This is here to avoid people getting screaming feedback
    //// when they first compile a plugin, but obviously you don't need to keep
    //// this code if your algorithm always overwrites all the output channels.
    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //    buffer.clear (i, 0, buffer.getNumSamples());

    //// This is the place where you'd normally do the guts of your plugin's
    //// audio processing...
    //// Make sure to reset the state if your inner loop is processing
    //// the samples and the outer loop is handling the channels.
    //// Alternatively, you can process the samples with the channels
    //// interleaved by keeping the same state.
    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);

    //    // ..do something to the data...
    //}

    buffer.clear();

    midiMessages.clear(); // this ensures we do not play midi coming from the DAW

    // Get current playhead position and transport state
    //if (auto* playHead = getPlayHead())
    //{
    //    juce::AudioPlayHead::CurrentPositionInfo positionInfo;
    //    if (playHead->getCurrentPosition(positionInfo))
    //    {
    //        isPlaying = positionInfo.isPlaying;
    //        bpm = positionInfo.bpm;
    //        // If the transport has just started playing, reset the current position and event index
    //        if (positionInfo.isPlaying && !wasPlaying)
    //        {
    //            currentPosition = 0.0;
    //            currentEventIndex = 0;
    //        }
    //        wasPlaying = positionInfo.isPlaying;
    //    }
    //}

    if (isPlaying || isPreviewing)
    {

        const double ticksPerQuarterNote = ppq;
        const double secondsPerQuarterNote = 60.0 / bpm;
        const double samplesPerQuarterNote = sampleRate * secondsPerQuarterNote;
        const double samplesPerTick = samplesPerQuarterNote / ticksPerQuarterNote;

        const int numSamples = buffer.getNumSamples();
        const double loopLengthInQuarters = 8 * 4; // 8 bars
        const double loopLengthInSamples = samplesPerQuarterNote * loopLengthInQuarters;

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Calculate the current position within the loop
            double loopPosition = fmod(currentPosition + (sample / samplesPerTick), loopLengthInSamples);

            while (midiSequence.getNumEvents() > currentEventIndex &&
                   midiSequence.getEventTime(currentEventIndex) < loopPosition)
            {
                const auto* midiEvent = midiSequence.getEventPointer(currentEventIndex);
                if (midiEvent->message.isNoteOnOrOff() || midiEvent->message.isController())
                {
                    midiMessages.addEvent(midiEvent->message, sample);
                }
                ++currentEventIndex;
            }

            // Reset the event index when reaching the end of the sequence
            if (currentEventIndex >= midiSequence.getNumEvents())
            {
                currentEventIndex = 0;
            }
        }

        currentPosition += numSamples / samplesPerTick;

        // Reset position and event index when reaching the end of the loop
        if (currentPosition >= loopLengthInSamples / samplesPerTick)
        {
            currentPosition -= loopLengthInSamples / samplesPerTick;
            currentEventIndex = 0;
            isPreviewing = false;
        }
    }

    // Process the MIDI messages to ensure they are handled by the synthesizer
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Clear the midiMessages buffer to prevent them from being sent to the system MIDI device
    midiMessages.clear();

}

//==============================================================================
bool SynergyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynergyAudioProcessor::createEditor()
{
    return new SynergyAudioProcessorEditor (*this);
}

//==============================================================================
void SynergyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(parameters.state.createXml());

    // Serialize the vector of MidiNotes
    juce::XmlElement* notesElement = xml->createNewChildElement("MidiNotes");
    for (const auto& note : midiNotes)
    {
        juce::XmlElement* noteElement = notesElement->createNewChildElement("MidiNote");
        noteElement->setAttribute("pitch", note.pitch);
        noteElement->setAttribute("octave", note.octave);
        noteElement->setAttribute("startBeat", note.startBeat);
        noteElement->setAttribute("lengthInBeats", note.lengthInBeats);
    }

    copyXmlToBinary(*xml, destData);
    
}

void SynergyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.state = juce::ValueTree::fromXml(*xmlState);

            // Deserialize the vector of MidiNotes
            midiNotes.clear();
            if (auto* notesElement = xmlState->getChildByName("MidiNotes"))
            {
                for (auto* noteElement : notesElement->getChildIterator())
                {
                    MidiNote note;
                    note.pitch = noteElement->getIntAttribute("pitch");
                    note.octave = noteElement->getIntAttribute("octave");
                    note.startBeat = (float)noteElement->getDoubleAttribute("startBeat");
                    note.lengthInBeats = (float)noteElement->getDoubleAttribute("lengthInBeats");
                    midiNotes.push_back(note);
                }
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynergyAudioProcessor();
}

void SynergyAudioProcessor::loadFile(const juce::String& path) 
{
    auto file = File(path);
    FileInputStream midiStream(file);
    midiFile.readFrom(midiStream);
}

void SynergyAudioProcessor::loadPreviewMidiFile(const juce::MidiMessageSequence midiSeq)
{
    midiSequence = midiSeq;

    DBG("Loaded MIDI sequence with " << midiSequence.getNumEvents() << " events.");

    for (int i = 0; i < midiSequence.getNumEvents(); ++i)
    {
        const auto* midiEvent = midiSequence.getEventPointer(i);
        DBG("Event " << i << ": timestamp = " << midiEvent->message.getTimeStamp());
    }
}

void SynergyAudioProcessor::playAudio()
{
    if (midiSequence.getNumEvents() > 0 && !isPreviewing)
    {
        //bpm = getMidiFileBPM();
        updateBPMFromHost();
        isPreviewing = true;
        currentPosition = 0;
        currentEventIndex = 0;
    }
    else isPreviewing = false;
}


void SynergyAudioProcessor::setBPM(double newBPM)
{
    bpm = newBPM;
}

void SynergyAudioProcessor::updateBPMFromHost()
{
    if (auto* playHead = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo info;
        if (playHead->getCurrentPosition(info))
        {
            bpm = info.bpm;
        }
    }
}

void SynergyAudioProcessor::setPreviewBass(int previewBass)
{
    synth.clearSounds(); // we clear sounds to make sure we don't have more than one sound playing at a time

    std::unique_ptr<juce::AudioFormatReader> audioFormatReader;

    switch (previewBass)
    {
    case 1:
        // outta space
        audioFormatReader = std::unique_ptr<juce::AudioFormatReader>(
            formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(
                BinaryData::Outta_Space_wav, BinaryData::Outta_Space_wavSize, false
            ))
        );

        if (audioFormatReader != nullptr)
        {
            juce::BigInteger allNotes;
            allNotes.setRange(0, 128, true);
            synth.addSound(new juce::SamplerSound("OuttaSpace", *audioFormatReader,
                                                  allNotes, 36, 0.1, 0.1, 5.0));
        }
        break;
    case 2:
        // Motion 808
        audioFormatReader = std::unique_ptr<juce::AudioFormatReader>(
            formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(
                BinaryData::Motion_808_wav, BinaryData::Motion_808_wavSize, false
            ))
        );

        if (audioFormatReader != nullptr)
        {
            juce::BigInteger allNotes;
            allNotes.setRange(0, 128, true);
            synth.addSound(new juce::SamplerSound("OuttaSpace", *audioFormatReader,
                                                  allNotes, 36, 0.1, 0.1, 5.0));
        }
        break;
    case 3:
        // Rock 808
        audioFormatReader = std::unique_ptr<juce::AudioFormatReader>(
            formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(
                BinaryData::Rock_808_wav, BinaryData::Rock_808_wavSize, false
            ))
        );

        if (audioFormatReader != nullptr)
        {
            juce::BigInteger allNotes;
            allNotes.setRange(0, 128, true);
            synth.addSound(new juce::SamplerSound("OuttaSpace", *audioFormatReader,
                                                  allNotes, 36, 0.1, 0.1, 5.0));
        }
        break;
    case 4:
        // WOW 808
        audioFormatReader = std::unique_ptr<juce::AudioFormatReader>(
            formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(
                BinaryData::Wow_808_wav, BinaryData::Wow_808_wavSize, false
            ))
        );

        if (audioFormatReader != nullptr)
        {
            juce::BigInteger allNotes;
            allNotes.setRange(0, 128, true);
            synth.addSound(new juce::SamplerSound("OuttaSpace", *audioFormatReader,
                                                  allNotes, 36, 0.1, 0.1, 5.0));
        }
        break;
    case 5:
        // Wub
        audioFormatReader = std::unique_ptr<juce::AudioFormatReader>(
            formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(
                BinaryData::Wub_wav, BinaryData::Wub_wavSize, false
            ))
        );

        if (audioFormatReader != nullptr)
        {
            juce::BigInteger allNotes;
            allNotes.setRange(0, 128, true);
            synth.addSound(new juce::SamplerSound("OuttaSpace", *audioFormatReader,
                                                  allNotes, 41, 0.1, 0.1, 5.0));
        }
        break;
    default:
        std::cerr << "Unrecognized previewBass code!" << std::endl;
    }
}

// Function to get the tempo in BPM from a MIDI file
double SynergyAudioProcessor::getMidiFileBPM()
{
    double bpm = 175.0; // Default tempo

    for (int trackIndex = 0; trackIndex < midiFile.getNumTracks(); ++trackIndex)
    {
        const juce::MidiMessageSequence* track = midiFile.getTrack(trackIndex);

        for (int eventIndex = 0; eventIndex < track->getNumEvents(); ++eventIndex)
        {
            const juce::MidiMessage& message = track->getEventPointer(eventIndex)->message;

            if (message.isTempoMetaEvent())
            {
                bpm = message.getTempoSecondsPerQuarterNote() > 0 ? 60.0 / message.getTempoSecondsPerQuarterNote() : 120.0;
                return bpm; // Assuming the first tempo event represents the BPM for the entire file
            }
        }
    }

    return bpm; // Return default or found BPM
}