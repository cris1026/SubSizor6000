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
#include <windows.foundation.numerics.h>

class SynthVoice : public juce::SynthesiserVoice

{
public:
    bool canPlaySound (juce::SynthesiserSound* sound)
    {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    //==============================================

    void startNote (int midiNoteNumber, float Velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
    {
        
        freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        std::cout << midiNoteNumber << std::endl;
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


private:
    double level;
    double freq;

};
