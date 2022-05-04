/*
  ==============================================================================

    SynthSound.h
    Created: 4 May 2022 5:03:59pm
    Author:  franb

    Contains the sounds which will be played by the synth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};

