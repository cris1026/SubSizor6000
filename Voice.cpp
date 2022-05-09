#include "Voice.h"

//CONSTRUCTOR

voice::voice(){}

voice::voice(int note, float freq, juce::ADSR::Parameters params, juce::ADSR::Parameters filterParams, double sampleRate, juce::dsp::ProcessSpec spec){
    this->note = note;
    this->freq = freq;
    
    this->sampleRate = sampleRate;

    this->env.setSampleRate(sampleRate);
    this->env.setParameters(params);

    this->filterEnv.setSampleRate(sampleRate);
    this->filterEnv.setParameters(filterParams);

}


void voice::operator = (const voice& v) {

    this->note = v.note;
    this->freq = v.freq;
    this->phase = v.phase;

    this->squareGain = v.squareGain;
    this->triangleGain = v.triangleGain;
    this->squareness = v.squareness;
    this->triangleness = v.triangleness;

    this->sampleRate = v.sampleRate;

    this->env.setSampleRate(v.sampleRate);
    this->env.setParameters(v.env.getParameters());

    this->filterEnv.setSampleRate(v.sampleRate);
    this->filterEnv.setParameters(v.filterEnv.getParameters());



}

//UTILITIES
int voice::getNote(){
    return this->note;
}

void voice::setNote(int val){
    this->note = val;
}
void voice::setFreq(float val){
    this->freq = val;
}

float voice::getFreq() {
    return freq;
}

juce::ADSR voice::getEnv() {
    return env;
}

void voice::startEnv(){
    this->env.noteOn();
}

void voice::endEnv(){
    this->env.noteOff();
}

void voice::resetEnv(){
    this->env.reset();
}


void voice::setSquareness(float val){
    this->squareness = val;
}
void voice::setTriangleness(float val){
    this->triangleness = val;
}

void voice::setSquareGain(float val){
    this->squareGain = val;
}
void voice::setTriangleGain(float val){
    this->triangleGain = val;
}

float voice::getPhase(){
    return this->phase;
}

//OSC PART
void voice::updatePhase(){
    phase += doublePi*(float)(freq/sampleRate);
    
    if(phase > doublePi)
        phase-=doublePi;
}

std::string voice::toString()
{
    return "Voice: [Freq = " + std::to_string(this->freq) + " isActive = " + std::to_string(env.isActive()) + "] VOI";
}

juce::AudioBuffer<float> voice::generateBuffer(int bufferSize){
    
    juce::AudioBuffer<float> buffer = juce::AudioBuffer<float>(1, bufferSize);
    
    float* channelDataL = buffer.getWritePointer(0);
    //float* channelDataR = buffer.getWritePointer(1);
    
    float currentSample;
    
    for(int i = 0; i<bufferSize; ++i){
        //Almost Sawtooth to Square wave
        currentSample = squareGain * (float)(((sin(phase)*tan(0.5*cos(phase)+1))/4.5+squareness*cos(phase))*3*log(squareness+2));
        //Sine to triangle wave
        currentSample += triangleGain * (2*sin(phase))/(2+triangleness*abs(cos(phase)));
        currentSample *= (float)env.getNextSample();
        
        channelDataL[i] = currentSample;
        
        updatePhase();
    }
    
    
    return buffer;
}

bool voice::isPlaying(){
    return env.isActive();
}

void voice::prepareToFilter(double sampleRate, int samplesPerBlock, int numChannels) {
    juce::dsp::ProcessSpec spec; //struct where some dsp process information are stored

    //lastSampleRate = sampleRate;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;

    duplicationFilter.reset();
    updateFilter(samplesPerBlock);
    duplicationFilter.prepare(spec);
}

void voice::updateFilter(int bufferSize) {
    /*
    This functions switches parameters and functionalities depending on the chosen type of filter: lowpass, highpass, bandpass
    */

    //This parameters must be controlled by the GUI
    int menuChoice = 0; //DUMMY
    float originCutOff = 5000; //DUMMY
    float cutOff = originCutOff; //DUMMY
    float resonance = 0.7; //DUMMY


    //WIP: cutOff
    cutOff = (originCutOff * ((pow(10,(float)filterEnv.getNextSample())-0.999)/9));
    //the filter must be applied once per buffer, every update we skip a number of samples.
    //This operation let the filter parameters to be correctly expressed in seconds, however it could be inefficient
  
    /*for (int i = 0; i < bufferSize; i++) {
        if (i == 0)
            cutOff = originCutOff * ((float)filterEnv.getNextSample() + 0.0001);
        else
            filterEnv.getNextSample();
    }*/
    
    //DBG(cutOff);

    //Choose the filter type
    if (menuChoice == 0) { //LowPass
        duplicationFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        duplicationFilter.state->setCutOffFrequency(sampleRate, cutOff, resonance);
    }
    else if (menuChoice == 1) //BandPass
    {
        duplicationFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        duplicationFilter.state->setCutOffFrequency(sampleRate, cutOff, resonance);
    }
    else //HighPass
    {
        duplicationFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        duplicationFilter.state->setCutOffFrequency(sampleRate, cutOff, resonance);
    }
}

void voice::processFilter(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    updateFilter(buffer.getNumSamples());
    duplicationFilter.process(juce::dsp::ProcessContextReplacing<float>(block));

}

void voice::startFilterEnv() {
    this->filterEnv.noteOn();
}
void voice::endFilterEnv() {
    this->filterEnv.noteOff();
}
void voice::resetFilterEnv() {
    this->filterEnv.reset();
}

