/*
  ==============================================================================

    OscData.cpp
    Created: 14 Feb 2021 6:51:17pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    resetAll();
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    prepare (spec);
    gain.prepare (spec);    
}

void OscData::setType (const int oscSelection)
{
    juce::File gradientFile("/Users/chasedurand/Dev/ScanlineSynth/Assets/gradient1.jpg"); // TODO replace with variable file selector
    auto gradientImage = juce::ImageCache::getFromFile(gradientFile);
    gradientImage = gradientImage.rescaled(1024, 1024);
    std::vector<int> scanRaw;
    switch (oscSelection)
    {
        // Red
        case 0:
            for (int i=1; i<=1024; i++){
                scanRaw.push_back(gradientImage.getPixelAt(i, 0).getRed());
            }
            break;
            
        // Blue
        case 1:
        {
            for (int i=1; i<=1024; i++){
                scanRaw.push_back(gradientImage.getPixelAt(i, 0).getBlue());
            }
            break;
        }
          
        // Green
        case 2:
        {
            for (int i=1; i<=1024; i++){
                scanRaw.push_back(gradientImage.getPixelAt(i, 0).getGreen());
            }
            break;
        }
        default:
            // You shouldn't be here!
            jassertfalse;
            break;
    }
    // Rescale range from min,max to -1.0,1.0
    auto scanMin = *std::min_element(scanRaw.begin(), scanRaw.end());
    auto scanMax = *std::max_element(scanRaw.begin(), scanRaw.end());
    float scanScaled[1024];
    for (int i=0; i < 1024; i++){
        scanScaled[i] = ((2.0)*(scanRaw[i]-scanMin) / (scanMax - scanMin)) -1.0;
    }
    initialise ([scanScaled](float x) { return scanScaled[(int) (((x+juce::MathConstants<float>::pi)*1023.0) / (2*juce::MathConstants<float>::pi))]; },1024); // TODO Replace with LERP
}

void OscData::setGain (const float levelInDecibels)
{
    gain.setGainDecibels (levelInDecibels);
}

void OscData::setOscPitch (const int pitch)
{
    lastPitch = pitch;
    setFrequency (juce::MidiMessage::getMidiNoteInHertz (lastMidiNote + lastPitch));

}

void OscData::setFreq (const int midiNoteNumber)
{
    setFrequency (juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber + lastPitch));
    lastMidiNote = midiNoteNumber;
}

void OscData::renderNextBlock (juce::dsp::AudioBlock<float>& audioBlock)
{
    jassert (audioBlock.getNumSamples() > 0);
    process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
}

float OscData::processNextSample (float input)
{
    return gain.processSample (processSample (input));
}

void OscData::setParams (const int oscChoice, const float oscGain, const int oscPitch)
{
    setType (oscChoice);
    setGain (oscGain);
    setOscPitch (oscPitch);
}

void OscData::resetAll()
{
    reset();
    gain.reset();
}
