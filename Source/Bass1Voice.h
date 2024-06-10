/*
  ==============================================================================

    Bass1Voice.h
    Created: 9 Jun 2024 1:46:56am
    Author:  skyla

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Bass1.h"

class Bass1Voice : public juce::SynthesiserVoice
{
public:
    Bass1Voice()
    {
        // Initialize the oscillators
        subOsc.initialise([](float x) { return std::sin(x); }, 128); // Sub bass oscillator
        midOsc.initialise([](float x) { return x / juce::MathConstants<float>::pi - 1.0f; }, 128); // Sawtooth wave for mid-range

        // Configure the ADSR envelope
        juce::ADSR::Parameters envParams;
        envParams.attack = 2.0f;
        envParams.decay = 0.1f;
        envParams.sustain = 0.8f;
        envParams.release = 0.01f;
        env.setParameters(envParams);
    }

    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<Bass1*>(sound) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* /*sound*/,
                    int /*currentPitchWheelPosition*/) override
    {
        float frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        subOsc.setFrequency(frequency);
        midOsc.setFrequency(frequency * 0.99f); // Slight detune for depth

        level = velocity;
        env.noteOn();
    }

    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        env.noteOff();

        if (!allowTailOff || !env.isActive())
            clearCurrentNote();
    }

    void pitchWheelMoved (int /*newPitchWheelValue*/) override {}
    void controllerMoved (int /*controllerNumber*/, int /*newControllerValue*/) override {}

    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        juce::AudioBuffer<float> tempBuffer;
        tempBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float subSample = subOsc.processSample(0.0f) * env.getNextSample() * level;
            float midSample = midOsc.processSample(0.0f) * env.getNextSample() * level;

            // Add a bit of saturation to the mid-range
            midSample = juce::jlimit(-1.0f, 1.0f, midSample * (1.0f + std::abs(midSample)));

            float currentSample = subSample + midSample * 0.3f; // Blend sub and mid-range

            for (int channel = 0; channel < tempBuffer.getNumChannels(); ++channel)
            {
                tempBuffer.setSample(channel, sample, currentSample);
            }
        }

        juce::dsp::AudioBlock<float> block(tempBuffer);
        juce::dsp::ProcessContextReplacing<float> context(block);

        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            auto monoBlock = block.getSingleChannelBlock(channel);
            juce::dsp::ProcessContextReplacing<float> monoContext(monoBlock);
            filters[channel].process(monoContext);
        }

        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addFrom(channel, startSample, tempBuffer, channel, 0, numSamples);
        }
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        juce::dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint32>(samplesPerBlock), 1 };

        for (auto& filter : filters)
        {
            filter.prepare(spec);
            filter.reset();
            filter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 350.0f);
        }
    }

private:
    juce::dsp::Oscillator<float> subOsc{ [](float x) { return std::sin(x); } }; // Sine wave for sub bass
    juce::dsp::Oscillator<float> midOsc{ [](float x) { return x / juce::MathConstants<float>::pi - 1.0f; } }; // Sawtooth wave for mid-range
    juce::ADSR env;
    std::array<juce::dsp::IIR::Filter<float>, 2> filters; // One filter per channel
    float level = 0.0f;
};