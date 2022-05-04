/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SubSizor6000AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SubSizor6000AudioProcessorEditor (SubSizor6000AudioProcessor&);
    ~SubSizor6000AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SubSizor6000AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubSizor6000AudioProcessorEditor)
};
