#pragma once

#include <JuceHeader.h>
#include <math.h>
#include <list>
#include "Voice.h"

const int voiceCount = 8;

//reorder methods to follow standard ordering in both this and cpp file
//(constructor, functional methods, getter/setters (same var together with
//setter first), tostring)
class SubSyzorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SubSyzorAudioProcessor();
    ~SubSyzorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void setSquareness(int val);
    void setTriangleness(int val);
    
    void setSquareGain(float val);
    void setTriangleGain(float val);
    
    void setVolume(float val){};
    
private:

    double currentSampleRate;
    

    juce::ADSR::Parameters envParams;
    juce::ADSR::Parameters filterEnvParams;
    
    //squareness range: [1,4]
    float squareness;
    //triangleness range: [0,4]
    float triangleness;
    
    float squareGain;
    float triangleGain;

    double doublePi = juce::MathConstants<double>::twoPi;

    voice osc[voiceCount];
    
    unsigned int age[voiceCount];
    
    juce::AudioBuffer<float> buffer_temp;
    juce::AudioBuffer<float> buffer_sum;

    juce::dsp::Gain<float> masterVolume;
    juce::dsp::ProcessSpec spec;
    
    //LFO STUFF
    float lfoPhase;
    float lfoAmp;
    float lfoDepth;
    float lfoFreq;
    bool lfoActive; //true: active, false: bypass
    bool lfoSwitch; //true: morph, false: pitch
    int note[voiceCount];

    float squarenessMod;
    float trianglenessMod;
    
    //PHAZOR
    bool phazorActive;
    juce::dsp::Phaser<float> phazor;
    float phRate;
    float phDepth;
    float phCenterFreq;
    float phFeedback;
    float phMix;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubSyzorAudioProcessor)
};
