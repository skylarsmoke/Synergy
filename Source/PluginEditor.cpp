/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <ctime>
#include <string>

using namespace std;
using namespace juce;

void SynergyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider->getComponentID() == "NoteVelocity")
    {
        noteVelocityValue.setText((juce::String)noteVelocitySlider.getValue(), juce::NotificationType::dontSendNotification);
    }
    else if (slider->getComponentID() == "Variety") {
        varietySliderValue.setText((juce::String)varietySlider.getValue(), juce::NotificationType::dontSendNotification);
    }
    else if (slider->getComponentID() == "Swing") {
        swingSliderValue.setText(((juce::String)swingSlider.getValue()) + "%", juce::NotificationType::dontSendNotification);
    }
    
}

bool SynergyAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::leftKey)
    {
        noteVelocitySlider.setValue(noteVelocitySlider.getValue() - 1, juce::sendNotificationSync);
        return true;
    }

    if (key == juce::KeyPress::rightKey)
    {
        noteVelocitySlider.setValue(noteVelocitySlider.getValue() + 1, juce::sendNotificationSync);
        return true;
    }

    if (key == juce::KeyPress::downKey)
    {
        varietySlider.setValue(varietySlider.getValue() - 1, juce::sendNotificationSync);
        return true;
    }

    if (key == juce::KeyPress::upKey)
    {
        varietySlider.setValue(varietySlider.getValue() + 1, juce::sendNotificationSync);
        return true;
    }

    return false;
}

//==============================================================================
SynergyAudioProcessorEditor::SynergyAudioProcessorEditor (SynergyAudioProcessor& p)
    : AudioProcessorEditor (&p), midiFileDrop (p, messageBox), audioProcessor (p), startTime (Time::getMillisecondCounterHiRes() * 0.001),
    productLockScreen(&productUnlockStatus, messageBox), generateButton(bassAI, midiViewer, stemTypeCombo.stemTypeCombo, selectKeyCombo.selectKeyCombo, viewport, varietySlider, p, settingsCache, noteVelocitySlider, swingSlider), 
    synergyLookAndFeel(&settingsCache), midiDragOutput(generateButton), midiViewer(midiDragOutput, 10)
{

    startTimer(100);
    
    setSize (900, 700);
    synergyFont.setBold(true);
    synergyFont.setHeight(14.0f);
    //synergyFont.setTypefaceName("Stars Fighters");
    

    // load settings
    loadSettings();
    
    // theme object
    theme = new Theme();
    
    setLookAndFeel(&synergyLookAndFeel);
    tooltipWindow.setLookAndFeel(&synergyLookAndFeel);

    // note velocity slider
    noteVelocitySlider.setSliderStyle(Slider::SliderStyle::LinearBar);
    noteVelocitySlider.setRange(0, 100, 1);
    noteVelocitySlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    noteVelocitySlider.setColour(Slider::textBoxTextColourId, Colours::transparentBlack);
    noteVelocitySlider.setValue(settingsCache.defaultNoteVelocity);
    noteVelocitySlider.setMouseCursor(MouseCursor::PointingHandCursor);
    noteVelocitySlider.setTextBoxIsEditable(false);
    noteVelocitySlider.setComponentID("NoteVelocity");
    noteVelocitySlider.addListener(this);
    noteVelocitySlider.setVelocityBasedMode(true);
    addAndMakeVisible(noteVelocitySlider);
    addAndMakeVisible(velocitySliderOverlay);
    velocitySliderOverlay.setInterceptsMouseClicks(false, false);
    
    noteVelocityValue.setText((juce::String)noteVelocitySlider.getValue(), juce::NotificationType::dontSendNotification);
    noteVelocityValue.setColour(juce::Label::textColourId, theme->mainSliderColor);
    noteVelocityValue.setJustificationType(Justification::centred);
    noteVelocityValue.setTooltip("Determines how many notes should be in the bassline.");
    addAndMakeVisible(noteVelocityValue);

    // variety slider
    varietySlider.setSliderStyle(Slider::SliderStyle::LinearBar);
    varietySlider.setRange(0, 100, 1);
    varietySlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    varietySlider.setColour(Slider::textBoxTextColourId, Colours::transparentBlack);
    varietySlider.setValue(settingsCache.defaultVariety);
    varietySlider.setMouseCursor(MouseCursor::PointingHandCursor);
    varietySlider.setTextBoxIsEditable(false);
    varietySlider.setComponentID("Variety");
    varietySlider.addListener(this);
    addAndMakeVisible(varietySlider);
    addAndMakeVisible(varietySliderOverlay);
    varietySliderOverlay.setInterceptsMouseClicks(false, false);

    varietySliderValue.setText((juce::String)varietySlider.getValue(), juce::NotificationType::dontSendNotification);
    varietySliderValue.setColour(juce::Label::textColourId, theme->mainSliderColor);
    varietySliderValue.setJustificationType(Justification::centred);
    varietySliderValue.setTooltip("Variety of notes generated. Higher values lead to more note generation and shorter notes.");
    addAndMakeVisible(varietySliderValue);

    // generate button
    addAndMakeVisible(generateButton);

    // settings button
    Image settingsButtonImage = ImageCache::getFromMemory(BinaryData::settingsIcon_png, BinaryData::settingsIcon_pngSize);
    Image settingsButtonImageHover = ImageCache::getFromMemory(BinaryData::settingsIconHover_png, BinaryData::settingsIconHover_pngSize);
    settingsButton.setImages(false, true, true, settingsButtonImage, 1.0f, {}, settingsButtonImageHover, 1.0f, {}, settingsButtonImage, 1.0f, {});
    settingsButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    settingsButton.onClick = [this] { openSettings(); };
    settingsButton.setTooltip("Settings");
    addAndMakeVisible(settingsButton);

    // preview button
    Image previewButtonImage = ImageCache::getFromMemory(BinaryData::previewIcon_png, BinaryData::previewIcon_pngSize);
    Image previewButtonImageHover = ImageCache::getFromMemory(BinaryData::previewIconHover_png, BinaryData::previewIconHover_pngSize);
    previewButton.setImages(false, true, true, previewButtonImage, 1.0f, {}, previewButtonImageHover, 1.0f, {}, previewButtonImage, 1.0f, {});
    previewButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    previewButton.setTooltip("Preview Bassline");
    previewButton.onClick = [this] { previewMidi(); };
    addAndMakeVisible(previewButton);

    // record button
    Image recordButtonImage = ImageCache::getFromMemory(BinaryData::refreshIcon_png, BinaryData::refreshIcon_pngSize);
    Image recordButtonImageHover = ImageCache::getFromMemory(BinaryData::refreshIconHover_png, BinaryData::refreshIconHover_pngSize);
    refreshButton.setImages(false, true, true, recordButtonImage, 1.0f, {}, recordButtonImageHover, 1.0f, {}, recordButtonImage, 1.0f, {});
    refreshButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    refreshButton.setTooltip("Refresh Midi");
    refreshButton.onClick = [this] { refreshMidi(); };
    addAndMakeVisible(refreshButton);

    // dev mode label
    devModeLabel.setText("DEVELOPMENT MODE", NotificationType::dontSendNotification);
    devModeLabel.setColour(Label::textColourId, Colour(0,195,255));
    devModeLabel.setFont(synergyFont);
    if (developmentMode) addAndMakeVisible(devModeLabel);

    // beta label
    betaLabel.setText("BETA", NotificationType::dontSendNotification);
    betaLabel.setColour(Label::textColourId, Colour(0, 195, 255));
    auto betaFont = juce::Font();
    betaFont.setHeight(18.0f);
    betaFont.setBold(true);
    betaLabel.setFont(betaFont);
    //if (!developmentMode) addAndMakeVisible(betaLabel);

    // stem type combo box
    addAndMakeVisible(stemTypeCombo);

    // select key combo box  
    addAndMakeVisible(selectKeyCombo);

    // defining parameters of our slider
    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Volume");
    midiVolume.setValue(1.0);
    midiVolume.setColour(1, juce::Colour(100,100,100));
    midiVolume.addListener(this);
    
    // add slider to editor
    //addAndMakeVisible(&midiVolume);

    // midi button
    bassDrumButton.setButtonText("Bass Drum (36)");
    bassDrumButton.onClick = [this] { setNoteNumber(60); };
    //addAndMakeVisible(bassDrumButton);

    // midi message box
    messageBox.setMultiLine(true);
    messageBox.setReturnKeyStartsNewLine(true);
    messageBox.setReadOnly(true);
    messageBox.setScrollbarsShown(true);
    messageBox.setCaretVisible(false);
    messageBox.setPopupMenuEnabled(true);
    messageBox.setColour(TextEditor::backgroundColourId, Colour(50, 50, 50));
    messageBox.setColour(TextEditor::outlineColourId, Colour(60, 60, 60));
    messageBox.setColour(TextEditor::shadowColourId, Colour(200, 200, 200));
    if (developmentMode) addAndMakeVisible(messageBox);

    // drag and drop midi
    addAndMakeVisible(midiFileDrop);
    
    // midi viewer
    viewport.setViewedComponent(&midiViewer, false);
    addChildComponent(viewport);

    // midi drag output
    addChildComponent(midiDragOutput);

    // swing slider
    swingSlider.setSliderStyle(Slider::SliderStyle::LinearBar);
    swingSlider.setRange(0, 100, 1);
    swingSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    swingSlider.setColour(Slider::textBoxTextColourId, Colours::transparentBlack);
    swingSlider.setValue(0);
    swingSlider.setMouseCursor(MouseCursor::PointingHandCursor);
    swingSlider.setTextBoxIsEditable(false);
    swingSlider.setComponentID("Swing");
    swingSlider.addListener(this);
    addAndMakeVisible(swingSlider);

    swingSliderValue.setText(((juce::String)swingSlider.getValue()) + "%", juce::NotificationType::dontSendNotification);
    swingSliderValue.setColour(juce::Label::textColourId, theme->mainSliderColor);
    swingSliderValue.setJustificationType(Justification::centred);
    swingSliderValue.setFont(13.0);
    swingSliderValue.setTooltip("The amount of swing added to the bassline.");
    addAndMakeVisible(swingSliderValue);

    // loop buttons
    switch (settingsCache.basslineLoop)
    {
    case 1:
        twoBarButtonClicked();
        break;
    case 2:
        fourBarButtonClicked();
        break;
    case 3:
        eightBarButtonClicked();
        break;
    default:
        fourBarButtonClicked();
    }

    twoBarLoopButton.setButtonText("2");
    twoBarLoopButton.setComponentID("2BarLoop");
    twoBarLoopButton.setMouseCursor(MouseCursor::PointingHandCursor);
    twoBarLoopButton.onClick = [this] { twoBarButtonClicked(); };
    twoBarLoopButton.setTooltip("Sets the loop length to 2 bars, ideal for short repeating patterns.");
    addAndMakeVisible(twoBarLoopButton);

    fourBarLoopButton.setButtonText("4");
    fourBarLoopButton.setComponentID("4BarLoop");
    fourBarLoopButton.setMouseCursor(MouseCursor::PointingHandCursor);
    fourBarLoopButton.onClick = [this] { fourBarButtonClicked(); };
    fourBarLoopButton.setTooltip("Sets the loop length to 4 bars, offering a balanced loop duration for most tracks.");
    addAndMakeVisible(fourBarLoopButton);

    eightBarLoopButton.setButtonText("8");
    eightBarLoopButton.setComponentID("8BarLoop");
    eightBarLoopButton.setMouseCursor(MouseCursor::PointingHandCursor);
    eightBarLoopButton.onClick = [this] { eightBarButtonClicked(); };
    eightBarLoopButton.setTooltip("Sets the loop length to 8 bars, great for longer, evolving musical phrases.");
    addAndMakeVisible(eightBarLoopButton);
    
    /*
    * Everything after this needs to be last
    */

    // product authentication
    //addChildComponent(productLockScreen);

    ////// product reactivation
    //productLockScreen.reactivate();

    //showUnlockForm();

    if (audioProcessor.midiNotes.size() > 0) {
        midiViewer.setMidiNotes(audioProcessor.midiNotes);
        viewport.setVisible(true);
        generateButton.createMidiFile(audioProcessor.midiNotes, audioProcessor.midiFile);
    }

    juce::Desktop::getInstance().setGlobalScaleFactor(1.0f);
}

SynergyAudioProcessorEditor::~SynergyAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    //tooltipWindow = nullptr;
}

//==============================================================================
void SynergyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(30,30,30));

    // background image
    Image backgroundImage;
    switch (theme->themeID) 
    {
        case 1:
            // default theme
            backgroundImage = juce::ImageCache::getFromMemory(BinaryData::SynergyBackgroundV1transformed_png, BinaryData::SynergyBackgroundV1transformed_pngSize);
            break;
        case 2:
            // black and white theme
            backgroundImage = juce::ImageCache::getFromMemory(BinaryData::SynergyBackgroundWhite_png, BinaryData::SynergyBackgroundWhite_pngSize);
            break;
    }

    g.drawImageWithin(backgroundImage, 0, 0, this->getWidth(), this->getHeight(), juce::RectanglePlacement::stretchToFit);
    

    Image settingsBorderImage = ImageCache::getFromMemory(BinaryData::settingsBordertransformed_png, BinaryData::settingsBordertransformed_pngSize);
    g.drawImageWithin(settingsBorderImage, 190, 280, settingsBorderImage.getWidth() - 190, settingsBorderImage.getHeight() - 190, RectanglePlacement::centred);

    // swing image
    Image swingImage = ImageCache::getFromMemory(BinaryData::SwingComponent_png, BinaryData::SwingComponent_pngSize);
    g.drawImageWithin(swingImage, 635, 315, swingImage.getWidth() - 60, swingImage.getHeight() - 60, RectanglePlacement::centred);

    Image loopImage = ImageCache::getFromMemory(BinaryData::LoopComponent_png, BinaryData::LoopComponent_pngSize);
    g.drawImageWithin(loopImage, 25, 295, loopImage.getWidth() - 60, loopImage.getHeight() - 60, RectanglePlacement::centred);

    // slider border
    g.setColour(Colour(20, 20, 20));
    //g.drawRect(120, 195, 110, 25, 5);

    g.setColour (Colour(100,100,100));
    g.setFont (15.0f);

    // version text
    string versionString = "Execution Log           Build v";
    if (developmentMode) g.drawFittedText(versionString.append(ProjectInfo::versionString), 0, 0, getWidth(), 30, juce::Justification::topRight, 1);
    
    g.setColour(theme->mainSliderColor);

    // Define the rectangle dimensions and corner radius
    float x = 728.0f;
    float y = 366.0f;
    float width = 60;  // Adjust to your component's width
    float height = 10;  // Adjust to your component's height
    float cornerRadius = 4.0f;  // Adjust this value to control the roundness of the corners

    // Draw the rounded rectangle outline
    g.drawRoundedRectangle(x, y, width, height, cornerRadius, 1.0f);  // The last parameter is the thickness of the outline

    // draw loop rectangle
    g.drawRect(104, 366, 62, 17, 1.0);
    
}

void SynergyAudioProcessorEditor::resized()
{

    auto desktopScale = juce::Desktop::getInstance().getGlobalScaleFactor();
    setTransform(juce::AffineTransform::scale(desktopScale));
    // product lock screen
    productLockScreen.setBounds(0, 0, 900, 700);

    // sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);

    auto halfWidth = getWidth() / 2;
    auto buttonsBounds = getLocalBounds().withWidth(halfWidth).reduced(10);
    bassDrumButton.setBounds(buttonsBounds.getX(), 10, buttonsBounds.getWidth(), 20);

    // dev tools
    messageBox.setBounds(690, 20, 200, 100);
    devModeLabel.setBounds(0, 0, 150, 15);
    betaLabel.setBounds(840, 10, 150, 15);

    // select key combo
    selectKeyCombo.setBounds(660, 260, 140, 40);

    // stem type combo
    stemTypeCombo.setBounds(100, 260, 140, 40);

    // generate button
    generateButton.setBounds(255, 130, 400, 200);

    // midi file drop
    midiFileDrop.setBounds(100, 410, 700, 200);

    // midi drag output
    midiDragOutput.setBounds(100, 410, 700, 200);

    // midi viewer
    viewport.setBounds(112, 415, 680, 200);
    
    // note velocity slider
    noteVelocitySlider.setBounds(62, 160, 127, 15);
    velocitySliderOverlay.setBounds(62, 160, 127, 20);
    noteVelocityValue.setBounds(100, 180, 50, 10);

    // variety slider
    varietySlider.setBounds(696, 165, 127, 15);
    varietySliderOverlay.setBounds(696, 165, 127, 20);
    varietySliderValue.setBounds(734, 185, 50, 10);

    // swing slider
    swingSlider.setBounds(728, 366, 60, 10);
    swingSliderValue.setBounds(788, 366, 40, 10);
    
    // settings button
    settingsButton.setBounds(375, 347, 35, 35);

    // preview button
    previewButton.setBounds(480, 348, 32, 32);

    // record button
    refreshButton.setBounds(431, 350, 28, 28);

    // loop buttons
    twoBarLoopButton.setBounds(105, 367, 20, 15);
    fourBarLoopButton.setBounds(125, 367, 20, 15);
    eightBarLoopButton.setBounds(145, 367, 20, 15);


}

void SynergyAudioProcessorEditor::openSettings() {

    // extra security measure
    verifyPluginIsActivated();

    // Create a new Settings component and manage it with std::unique_ptr
    settings = std::make_unique<Settings>(settingsCache, audioProcessor);
    settings->setComponentID("Settings");
    settings->setUsingNativeTitleBar(true);
    settings->centreWithSize(500, 400);
    settings->setVisible(true);

    // Add the Settings component to the desktop so it gets its own window
    juce::DialogWindow::LaunchOptions options;
    options.content.setOwned(settings.release()); // Transfer ownership to the DialogWindow
    options.content->setSize(498, 370);
    options.dialogTitle = "Synergy Bass Settings";
    options.dialogBackgroundColour = juce::Colours::lightgrey;
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = true;
    options.resizable = false;

    options.launchAsync();

    
}

void SynergyAudioProcessorEditor::verifyPluginIsActivated()
{
    if (!productUnlockStatus.isUnlocked()) showUnlockForm();
}

void SynergyAudioProcessorEditor::timerCallback() 
{
    if (!isUnlocked && productUnlockStatus.isUnlocked())
    {
        isUnlocked = true;
        unlockPlugin();
    }
}

void SynergyAudioProcessorEditor::unlockPlugin()
{
    productLockScreen.dismiss();
}

void SynergyAudioProcessorEditor::showUnlockForm() 
{
    productLockScreen.setVisible(true);
}

void SynergyAudioProcessorEditor::previewMidi()
{
    audioProcessor.playAudio();
}

void SynergyAudioProcessorEditor::loadSettings()
{
    // Define the file path for loading settings
    File settingsFile(File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("sbr_settings.txt"));

    if (!settingsFile.existsAsFile())
    {
        DBG("Settings data is empty.");
        // this is ok and will happen until users change the default settings
        // we set default values when this happens

        settingsCache.basslineLoop = 2;
        settingsCache.defaultVariety = 100;
        settingsCache.defaultNoteVelocity = 100;
        settingsCache.previewBass = 3;
        settingsCache.toolTips = true;
    }
    else
    {
        // Read the JSON string from the file
        String jsonString = settingsFile.loadFileAsString();

        // Parse the JSON string into a var object
        var jsonVar = JSON::parse(jsonString);

        if (jsonVar.isVoid())
        {
            DBG("Failed to parse JSON.");
        }
        else
        {
            if (auto* jsonData = jsonVar.getDynamicObject())
            {
                settingsCache.basslineLoop = static_cast<int>(jsonData->getProperty("basslineLoop"));
                settingsCache.defaultVariety = static_cast<int>(jsonData->getProperty("defaultVariety"));
                settingsCache.defaultNoteVelocity = static_cast<int>(jsonData->getProperty("defaultNoteVelocity"));
                settingsCache.previewBass = static_cast<int>(jsonData->getProperty("previewBass"));
                settingsCache.toolTips = static_cast<bool>(jsonData->getProperty("toolTips"));
                DBG("Settings loaded successfully.");
            }
            else
            {
                DBG("Invalid JSON structure.");
            }
        }
    }

    // set preview bass
    audioProcessor.setPreviewBass(settingsCache.previewBass);
    
}

bool SynergyAudioProcessorEditor::tooltipsEnabled()
{
    return settingsCache.toolTips;
}

void SynergyAudioProcessorEditor::refreshMidi()
{
    viewport.setVisible(false);
    audioProcessor.midiFile.clear();
    midiFileDrop.midiLoaded = false;
    midiFileDrop.repaint();
    audioProcessor.midiSequence.clear();
}

void SynergyAudioProcessorEditor::twoBarButtonClicked()
{
    // Deactivate other buttons
    fourBarLoopButton.setColour(TextButton::buttonColourId, Colour(20, 20, 20));
    fourBarLoopButton.setColour(TextButton::buttonOnColourId, Colour(20, 20, 20));
    fourBarLoopButton.setColour(Label::textColourId, theme->mainSliderColor);

    eightBarLoopButton.setColour(TextButton::buttonColourId, Colour(20, 20, 20));
    eightBarLoopButton.setColour(TextButton::buttonOnColourId, Colour(20, 20, 20));
    eightBarLoopButton.setColour(Label::textColourId, theme->mainSliderColor);

    // Activate the clicked button
    twoBarLoopButton.setColour(TextButton::buttonColourId, theme->mainSliderColor);
    twoBarLoopButton.setColour(TextButton::buttonOnColourId, theme->mainSliderColor);
    twoBarLoopButton.setColour(Label::textColourId, Colour(20, 20, 20));

    // set bassline loop
    settingsCache.activeLoopLength = 1;
}

void SynergyAudioProcessorEditor::fourBarButtonClicked()
{
    // Deactivate other buttons
    twoBarLoopButton.setColour(TextButton::buttonColourId, Colour(20, 20, 20));
    twoBarLoopButton.setColour(TextButton::buttonOnColourId, Colour(20, 20, 20));
    twoBarLoopButton.setColour(Label::textColourId, theme->mainSliderColor);


    eightBarLoopButton.setColour(TextButton::buttonColourId, Colour(20, 20, 20));
    eightBarLoopButton.setColour(TextButton::buttonOnColourId, Colour(20, 20, 20));
    eightBarLoopButton.setColour(Label::textColourId, theme->mainSliderColor);

    // Activate the clicked button
    fourBarLoopButton.setColour(TextButton::buttonColourId, theme->mainSliderColor);
    fourBarLoopButton.setColour(TextButton::buttonOnColourId, theme->mainSliderColor);
    fourBarLoopButton.setColour(Label::textColourId, Colour(20, 20, 20));

    // set bassline loop
    settingsCache.activeLoopLength = 2;

}

void SynergyAudioProcessorEditor::eightBarButtonClicked()
{
    // Deactivate other buttons
    twoBarLoopButton.setColour(TextButton::buttonColourId, Colour(20, 20, 20));
    twoBarLoopButton.setColour(TextButton::buttonOnColourId, Colour(20, 20, 20));
    twoBarLoopButton.setColour(Label::textColourId, theme->mainSliderColor);

    fourBarLoopButton.setColour(TextButton::buttonColourId, Colour(20, 20, 20));
    fourBarLoopButton.setColour(TextButton::buttonOnColourId, Colour(20, 20, 20));
    fourBarLoopButton.setColour(Label::textColourId, theme->mainSliderColor);

    // Activate the clicked button
    eightBarLoopButton.setColour(TextButton::buttonColourId, theme->mainSliderColor);
    eightBarLoopButton.setColour(TextButton::buttonOnColourId, theme->mainSliderColor);
    eightBarLoopButton.setColour(Label::textColourId, Colour(20, 20, 20));

    // set bassline loop
    settingsCache.activeLoopLength = 3;

}
