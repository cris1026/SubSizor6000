#pragma once
#include <JuceHeader.h>
#include <string>

//reorder methods to follow standard ordering in both this and cpp file
//(constructor, functional methods, getter/setters (same var together with
//setter first), tostring)
class voice: private juce::ADSR
{
public:
    voice();
    voice(int note, float freq, juce::ADSR::Parameters params, juce::ADSR::Parameters filterParam, double sampleRate, juce::dsp::ProcessSpec spec);

    void operator = (const voice& v);

    int getNote();
    void setNote(int val);
    void setFreq(float val);
    
    void startEnv();
    void endEnv();
    void resetEnv();

    float getFreq();

    juce::ADSR getEnv();
    
    juce::AudioBuffer<float> generateBuffer(int bufferSize);
    
    void setSquareness(float val);
    void setTriangleness(float val);
    
    void setSquareGain(float val);
    void setTriangleGain(float val);

    bool isPlaying();
    
    void updatePhase();
    
    float getPhase();

    std::string toString();

    //---- Filter functions
    //NOTA: manda nell'update filter i Parameters moltiplicati per l'ADSR nella funzione processFilter
    void prepareToFilter(double sampleRate, int samplesPerBlock, int numChannels);
    void updateFilter(int bufferSize); //Note: must be called everytime filter parameters are changed, still not implemented
    void processFilter(juce::AudioBuffer<float>& buffer);

    void startFilterEnv();
    void endFilterEnv();
    void resetFilterEnv();
    
private:
    float doublePi = juce::MathConstants<float>::twoPi;

    int note;
    float freq;
    float phase = 0.0;
    
    float squareGain;
    float triangleGain;
    
    float squareness;
    float triangleness;

    double sampleRate;
    juce::ADSR env;
    juce::ADSR filterEnv;

    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>,
        juce::dsp::StateVariableFilter::Parameters<float>> duplicationFilter;
};
