/*
  ==============================================================================

    SynthSound.cpp
    Created: 4 May 2022 5:03:59pm
    Author:  franb

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound*) {
    return true;
}
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {

}
void SynthVoice::stopNote(float velocity, bool allowTailOff) {

}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}
void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) {

}
