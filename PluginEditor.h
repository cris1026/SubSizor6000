#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SubSyzorAudioProcessorEditor  :
    public juce::AudioProcessorEditor,
    private juce::Slider::Listener
{
public:
    SubSyzorAudioProcessorEditor (SubSyzorAudioProcessor&);
    ~SubSyzorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SubSyzorAudioProcessor& audioProcessor;
    
    //WAVE PARAMETERS
    juce::Slider squarenessSlider;
    juce::Slider trianglenessSlider;
    
    juce::Label squarenessLabel;
    juce::Label trianglenessLabel;

    //VOLUME & GAIN FOR EACH OSCILLATOR VOICE
    juce::Slider squareGainSlider;
    juce::Slider triangleGainSlider;
    juce::Slider volumeSlider;
    
    juce::Label squareGainLabel;
    juce::Label triangleGainLabel;
    juce::Label volumeLabel;
    
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubSyzorAudioProcessorEditor)
};
