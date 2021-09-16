/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapSynthAudioProcessorEditor::TapSynthAudioProcessorEditor (TapSynthAudioProcessor& p)
: AudioProcessorEditor (&p)
, audioProcessor (p)
, osc1 (audioProcessor.apvts, "OSC1", "OSC1GAIN", "OSC1PITCH")
, osc2 (audioProcessor.apvts, "OSC2", "OSC2GAIN", "OSC2PITCH")
, adsr (audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE")
{
    auto tapImage = juce::ImageCache::getFromMemory (BinaryData::tapLogo_png, BinaryData::tapLogo_pngSize);
    
    if (tapImage.isValid())
        logo.setImage (tapImage, juce::RectanglePlacement::stretchToFit);
    else
        jassertfalse;
    
    addAndMakeVisible (osc1);
    addAndMakeVisible (osc2);
    addAndMakeVisible (adsr);
    addAndMakeVisible (logo);
    
    osc1.setName ("Oscillator 1");
    osc2.setName ("Oscillator 2");
    adsr.setName ("ADSR");
    
    auto oscColour = juce::Colour::fromRGB (247, 190, 67);
    
    osc1.setBoundsColour (oscColour);
    osc2.setBoundsColour (oscColour);
        
    
    startTimerHz (30);
    setSize (1080, 525);
}

TapSynthAudioProcessorEditor::~TapSynthAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void TapSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void TapSynthAudioProcessorEditor::resized()
{
    const auto oscWidth = 420;
    const auto oscHeight = 180;
    osc1.setBounds (0, 0, oscWidth, oscHeight);
    osc2.setBounds (0, osc1.getBottom(), oscWidth, oscHeight);
    adsr.setBounds (osc1.getRight(), 0, 230, 360);
    logo.setBounds (osc2.getBottom(), osc2.getBottom() + 30, 250, 100);
}

void TapSynthAudioProcessorEditor::timerCallback()
{
    repaint();
}
