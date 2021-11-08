/*
  ==============================================================================

    OscData.h
    Created: 14 Feb 2021 6:51:17pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void setType (const int oscSelection);
    void setGain (const float levelInDecibels);
    void setOscPitch (const int pitch);
    void setFreq (const int midiNoteNumber);
    void renderNextBlock (juce::dsp::AudioBlock<float>& audioBlock);
    float processNextSample (float input);
    void setParams (const int oscChoice, const float oscGain, const int oscPitch);
    void resetAll();

private:
    juce::dsp::Gain<float> gain;
    int lastPitch { 0 };
    int lastMidiNote { 0 };
};
