#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    addAndMakeVisible (renderer);
    setSize (400, 300);
}
AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}
//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g) { juce::ignoreUnused (g); }
void AudioPluginAudioProcessorEditor::resized() { renderer.setBounds (getLocalBounds()); }