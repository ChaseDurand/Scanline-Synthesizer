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
, osc3 (audioProcessor.apvts, "OSC3", "OSC3GAIN", "OSC3PITCH")
, adsr (audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE")
{
    
    juce::File gradientFile("/Users/chasedurand/Dev/ScanlineSynth/Assets/gradient1.jpg");
    
    auto gradientImage = juce::ImageCache::getFromFile(gradientFile);
    gradientImage = gradientImage.rescaled(1024, 1024);
    std::cout << gradientImage.getPixelAt(0, 0).getRed();
    
    if (gradientImage.isValid())
        gradient.setImage(gradientImage, juce::RectanglePlacement::stretchToFit);
    else
        jassertfalse;
    
    addAndMakeVisible (osc1);
    addAndMakeVisible (osc2);
    addAndMakeVisible (osc3);
    addAndMakeVisible (adsr);
    addAndMakeVisible (gradient);
    osc1.setName ("Oscillator 1");
    osc2.setName ("Oscillator 2");
    osc3.setName ("Oscillator 3");
    adsr.setName ("ADSR");
    
    auto oscColour = juce::Colour::fromRGB (247, 190, 67);
    
    osc1.setBoundsColour (oscColour);
    osc2.setBoundsColour (oscColour);
    osc3.setBoundsColour (oscColour);
        
    
    startTimerHz (30);
    setSize (800, 600);
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
    int margin = 5;
    
    gradient.setBounds(margin, margin, 350, 350);
    
    const auto oscWidth = 350;
    const auto oscHeight = 150;
    osc1.setBounds (gradient.getRight()+margin, margin, oscWidth, oscHeight);
    osc2.setBounds (gradient.getRight()+margin, osc1.getBottom()+margin, oscWidth, oscHeight);
    osc3.setBounds (gradient.getRight()+margin, osc2.getBottom()+margin, oscWidth, oscHeight);
    adsr.setBounds (margin, gradient.getBottom()+margin, 350, 150);
}

void TapSynthAudioProcessorEditor::timerCallback()
{
    repaint();
}
