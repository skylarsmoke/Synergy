/*
  ==============================================================================

    DefaultSliderFilter.h
    Created: 9 Jun 2024 8:51:34pm
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DefaultSliderFilter : public juce::TextEditor::Listener
{
public:
    void textEditorTextChanged(juce::TextEditor& editor) override
    {
        juce::String text = editor.getText();
        juce::String filteredText;

        // Filter out non-numeric characters
        for (auto c : text)
        {
            if (juce::CharacterFunctions::isDigit(c))
                filteredText += c;
        }

        // Remove leading zeros
        while (filteredText.startsWithChar('0') && filteredText.length() > 1)
        {
            filteredText = filteredText.substring(1);
        }

        if (filteredText.isEmpty())
        {
            filteredText = "0";
        }
        else
        {
            int value = filteredText.getIntValue();
            if (value < 0)
                filteredText = "0";
            else if (value > 100)
                filteredText = "100";
        }

        if (text != filteredText)
        {
            editor.setText(filteredText, juce::dontSendNotification);
            editor.setCaretPosition(filteredText.length());
            
        }

        editor.setJustification(juce::Justification::centredTop);
        editor.setFont(14.0f);
    }
};
