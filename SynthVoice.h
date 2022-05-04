/*
  ==============================================================================

    SynthVoice.h
    Created: 4 May 2022 5:04:23pm
    Author:  franb

    A single synth voice, playing the sound provided by SynthSound

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthVoice : public juce::SynthesiserVoice {
public:
    bool canPlaySound(juce::SynthesiserSound*) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;

};


