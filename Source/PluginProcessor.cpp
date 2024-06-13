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
                       )
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

    if (isPlaying)
    {

        const double ticksPerQuarterNote = ppq;
        const double secondsPerQuarterNote = 60.0 / bpm;
        const double samplesPerQuarterNote = sampleRate * secondsPerQuarterNote;
        const double samplesPerTick = samplesPerQuarterNote / ticksPerQuarterNote;

        const int numSamples = buffer.getNumSamples();

        for (int sample = 0; sample < numSamples; ++sample)
        {
            while (midiSequence.getNumEvents() > currentEventIndex &&
                   midiSequence.getEventTime(currentEventIndex) < currentPosition + (sample / samplesPerTick))
            {
                const auto* midiEvent = midiSequence.getEventPointer(currentEventIndex);
                if (midiEvent->message.isNoteOnOrOff() || midiEvent->message.isController())
                {
                    midiMessages.addEvent(midiEvent->message, sample);
                }
                ++currentEventIndex;
            }
        }

        currentPosition += numSamples / samplesPerTick;

        if (currentEventIndex >= midiSequence.getNumEvents())
        {
            isPlaying = false; // Stop playing once all events have been processed
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
}

void SynergyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
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
    if (midiSequence.getNumEvents() > 0)
    {
        bpm = getMidiFileBPM();
        isPlaying = true;
        currentPosition = 0;
        currentEventIndex = 0;
    }
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