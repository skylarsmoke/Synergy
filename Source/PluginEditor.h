
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Theme.h"
#include "SynergyLookAndFeel.h"
#include "MidiFileDrop.h"
#include "GenerateButton.h"
#include "StemTypeCombo.h"
#include "SelectKeyComboBox.h"
#include "VelocitySliderOverlay.h"
#include "Settings.h"
#include "ProductUnlock.h"
#include "ProductLockScreen.h"
#include "MidiViewer.h"
#include "BassGenerator.h"
#include "GenSplash.h"
#include "SettingsCache.h"

//==============================================================================
/**
*/
class SynergyAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener, juce::Timer
{
public:
    SynergyAudioProcessorEditor (SynergyAudioProcessor&);
    ~SynergyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    SynergyLookAndFeel synergyLookAndFeel;
    MidiFileDrop midiFileDrop;
    GenerateButton generateButton;
    MidiViewer midiViewer;
    BassGenerator bassAI;

    std::unique_ptr<MidiViewer> midiGrid;
    juce::Viewport viewport;
    
    // uncomment to enable tool tips
    //TooltipWindow tooltips;

    // plugin activation
    ProductLockScreen productLockScreen;
    ProductUnlock productUnlockStatus;
    
    bool isUnlocked = false;

    void sliderValueChanged(juce::Slider* slider) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynergyAudioProcessor& audioProcessor;

    // theme object
    Theme* theme;

    juce::Slider midiVolume;
    bool developmentMode = false;

    // midi reading
    int note = 36;
    int midiChannel = 10;
    double startTime;
    juce::TextButton bassDrumButton;
    juce::TextEditor messageBox;
    juce::Label devModeLabel;

    // beta label
    juce::Label betaLabel;
    
    // combo boxes
    SelectKeyComboBox selectKeyCombo;
    StemTypeCombo stemTypeCombo;

    // record panel
    juce::ImageButton settingsButton;
    juce::ImageButton previewButton;
    juce::ImageButton recordButton;

    // sliders
    juce::Slider noteVelocitySlider;
    juce::Slider varietySlider;
    VelocitySliderOverlay velocitySliderOverlay;
    VelocitySliderOverlay varietySliderOverlay;
    juce::Label noteVelocityValue;
    juce::Label varietySliderValue;

    // main font
    juce::Font synergyFont;

    // settings cache
    SettingsCache settingsCache;

    /// <summary>
    /// Opens the settings window
    /// </summary>
    void openSettings();

    // every 12 ints is an octave
    void setNoteNumber(int noteNumber) {
        auto message = juce::MidiMessage::noteOn(midiChannel, noteNumber, (juce::uint8)100);
        message.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
        addMessageToList(message);

        // immediately adding a note off because it is good practice to add a note off for each note on
        auto messageOff = juce::MidiMessage::noteOff(message.getChannel(), message.getNoteNumber());
        messageOff.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
        addMessageToList(messageOff);
    }

    void addMessageToList(const juce::MidiMessage& message) {
        auto time = message.getTimeStamp();

        auto hours = ((int)(time / 3600.0)) % 24;
        auto minutes = ((int)(time / 60.0)) % 60;
        auto seconds = ((int)time) % 60;
        auto millis = ((int)(time * 1000.0)) % 1000;

        auto timecode = juce::String::formatted("%02d:%02d:%02d.%03d",
                                                hours,
                                                minutes,
                                                seconds,
                                                millis);

        logMessage(timecode + " - " + getMidiMessageDecription(message));
    }

    /// <summary>
    /// Logs data in the message box
    /// </summary>
    /// <param name="m"></param>
    void logMessage(const juce::String& m) {
        messageBox.moveCaretToEnd();
        messageBox.insertTextAtCaret(m + juce::newLine);
    }

    /// <summary>
    /// Parses the MIDI data to get a human-readable description of the message
    /// </summary>
    /// <param name="message"></param>
    /// <returns></returns>
    static juce::String getMidiMessageDecription(const juce::MidiMessage& midiData) {
        if (midiData.isNoteOn()) return "Note on " + juce::MidiMessage::getMidiNoteName(midiData.getNoteNumber(), true, true, 3);
        if (midiData.isNoteOff()) return "Note off " + juce::MidiMessage::getMidiNoteName(midiData.getNoteNumber(), true, true, 3);
        if (midiData.isProgramChange()) return "Program change " + juce::String(midiData.getProgramChangeNumber());
        if (midiData.isPitchWheel()) return "Pitch wheel " + juce::String(midiData.getPitchWheelValue());
        if (midiData.isAftertouch()) return "After touch " + juce::MidiMessage::getMidiNoteName(midiData.getNoteNumber(), true, true, 3) + ": " + juce::String(midiData.getAfterTouchValue());
        if (midiData.isChannelPressure()) return "Channel pressure " + juce::String(midiData.getChannelPressureValue());
        if (midiData.isAllNotesOff()) return "All notes off";
        if (midiData.isAllSoundOff()) return "All sound off";
        if (midiData.isMetaEvent()) return "Meta event";

        if (midiData.isController()) {
            juce::String name(juce::MidiMessage::getControllerName(midiData.getControllerNumber()));

            if (name.isEmpty()) name = "[" + juce::String(midiData.getControllerNumber()) + "]";
            return "Controller " + name + ": " + juce::String(midiData.getControllerValue());
        }

        return juce::String::toHexString(midiData.getRawData(), midiData.getRawDataSize());
    }

    void timerCallback() override;
    void unlockPlugin();
    void showUnlockForm();
    void verifyPluginIsActivated();
    void previewMidi();
    
    std::unique_ptr<Settings> settings;

    /// <summary>
    /// Loads saved settings
    /// </summary>
    void loadSettings();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynergyAudioProcessorEditor)
};
