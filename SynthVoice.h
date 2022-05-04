/*
  ==============================================================================

    SynthVoice.h
    Created: 4 May 2022 2:33:26pm
    Author:  mcris

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice

{
public:
    bool canPlaySound (juce::SynthesiserSound* sound)
    {

    }

    //==============================================

    void startNote (int midiNoteNumber, float Velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
    {

    }

    //==============================================

    void stopNote (float velocity, bool allowTailOff)
    {

    }

    //==============================================

    void pitchWheelMoved (int newPitchWheelValue)
    {

    }

    //==============================================

    void controllerMoved(int controllerNumber, int newControllerValue)
    {

    }

    //==============================================

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
    {

    }

    //==============================================
};