/*
  ==============================================================================

    SynthSound.h
    Created: 4 May 2022 2:33:15pm
    Author:  mcris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class SynthSound : public juce::SynthesiserSound

{
public:
    bool appliesToNote (int /*midiNoteNumber*/)
    {
        return true;
    }
    
    bool appliesToChannel(int /*midiChannel*/)
    {
        return true;
    }
};
