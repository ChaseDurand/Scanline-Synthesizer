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
    switch (oscSelection)
    {
        // Sine
        case 0:
            initialise ([](float x) { return 1.5 * std::tanh(std::sin (x)); });
            break;
            
        // Saw
        case 1:
        {
            // initialise ([] (float x) { return x / juce::MathConstants<float>::pi; });
            //initialise ([] (float x) { return x / juce::MathConstants<float>::pi; }, 4);
            //initialise ([](float x) { return 10.0 * std::tanh(std::sin (x)); },8);
            
            
            juce::File gradientFile("/Users/chasedurand/Dev/ScanlineSynth/Assets/gradient1.jpg");
            auto gradientImage = juce::ImageCache::getFromFile(gradientFile);
            gradientImage = gradientImage.rescaled(1024, 1024);
            std::vector<int> scanRed;
            //int scanRed[1024]={};
            for (int i=1; i<=1024; i++){
                scanRed.push_back(gradientImage.getPixelAt(i, 0).getRed());
            }
            // Rescale range from min,max to -1.0,1.0
            auto redMin = *std::min_element(scanRed.begin(), scanRed.end());
            auto redMax = *std::max_element(scanRed.begin(), scanRed.end());
            std::cout << redMin << redMax << std::endl;
            float scaledRed[1024];
            for (int i=0; i < 1024; i++){
                scaledRed[i] = ((2.0)*(scanRed[i]-redMin) / (redMax - redMin)) -1.0;
            }
            initialise ([scaledRed](float x) { return scaledRed[(int) (((x+juce::MathConstants<float>::pi)*1023.0) / (2*juce::MathConstants<float>::pi))]; },1024); // Replace with LERP
            break;
        }
          
        // Square
        case 2:
        {
            // initialise ([] (float x) { return x < 0.0f ? -1.0f : 1.0f; });
            
            
            
            juce::File gradientFile("/Users/chasedurand/Dev/ScanlineSynth/Assets/gradient1.jpg");
            auto gradientImage = juce::ImageCache::getFromFile(gradientFile);
            gradientImage = gradientImage.rescaled(1024, 1024);
            std::vector<int> scanRed;
            //int scanRed[1024]={};
            for (int i=1; i<=1024; i++){
                scanRed.push_back(gradientImage.getPixelAt(i, 0).getBlue());
            }
            // Rescale range from min,max to -1.0,1.0
            auto redMin = *std::min_element(scanRed.begin(), scanRed.end());
            auto redMax = *std::max_element(scanRed.begin(), scanRed.end());
            std::cout << redMin << redMax << std::endl;
            float scaledRed[1024];
            for (int i=0; i < 1024; i++){
                scaledRed[i] = ((2.0)*(scanRed[i]-redMin) / (redMax - redMin)) -1.0;
            }
            initialise ([scaledRed](float x) { return scaledRed[(int) (((x+juce::MathConstants<float>::pi)*1023.0) / (2*juce::MathConstants<float>::pi))]; },1024); // Replace with LERP
            
            break;
        }
        default:
            // You shouldn't be here!
            jassertfalse;
            break;
    }
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
