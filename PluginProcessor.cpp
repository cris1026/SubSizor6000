#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubSyzorAudioProcessor::SubSyzorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SubSyzorAudioProcessor::~SubSyzorAudioProcessor()
{
}

//==============================================================================
const juce::String SubSyzorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SubSyzorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SubSyzorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SubSyzorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SubSyzorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SubSyzorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SubSyzorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SubSyzorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SubSyzorAudioProcessor::getProgramName (int index)
{
    return {};
}

void SubSyzorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SubSyzorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //STANDARD OSC PARAMETERS
    squareness = 2.0;
    triangleness = 2.0;
    squareGain = 0.7;
    triangleGain = 0.0;
    
    //AMPLITUDE ADSR INITIALIZATION
    envParams.attack = 0.1;
    envParams.decay = 0.2;
    envParams.sustain = 0.7;
    envParams.release = 2.0;

    //FILTER ADSR INITIALIZATION
    filterEnvParams.attack = 0.0 / samplesPerBlock;
    filterEnvParams.decay = 0.0 / samplesPerBlock;
    filterEnvParams.sustain = 1;
    filterEnvParams.release = 2.0 / samplesPerBlock;
    
    //DSP STUFF INITIALIZATION
    currentSampleRate = sampleRate;
    
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;
    spec.maximumBlockSize = samplesPerBlock;
    
    for (int i = 0; i < voiceCount; i++) {
        
        osc[i] = voice(0, 0.0, envParams, filterEnvParams, sampleRate, spec);
        osc[i].setSquareness(squareness);
        osc[i].setTriangleness(triangleness);
        osc[i].setSquareGain(squareGain);
        osc[i].setTriangleGain(triangleGain);
        
        age[i] = 0;

        osc[i].prepareToFilter(sampleRate, samplesPerBlock, this->getMainBusNumOutputChannels());
    }
    
    buffer_temp = juce::AudioBuffer<float>(1, samplesPerBlock);
    buffer_sum = juce::AudioBuffer<float>(1, samplesPerBlock);
    
    //LFO INITIALIZATION
    lfoActive = true;
    lfoSwitch = true;
    lfoAmp = 1.0;
    lfoDepth = 1.0; //From 0 to 1
    lfoFreq = 1.0;
    lfoPhase = 0.0;
    squarenessMod = 0.0;
    trianglenessMod = 0.0;
    
    //PHAZOR INITIALIZATION
    phazorActive = false;
    phRate = 0.5;
    phDepth = 1.0;
    phCenterFreq = 440.0;
    phFeedback = 0.5;
    phMix = 0.7;
    
    phazor.reset();
    phazor.setRate(phRate);
    phazor.setDepth(phDepth);
    phazor.setCentreFrequency(phCenterFreq);
    phazor.setFeedback(phFeedback);
    phazor.setMix(phMix);
    phazor.prepare(spec);
    
    
    //MASTER VOLUME
    masterVolume.prepare(spec);
    masterVolume.setGainLinear(0.1);


}

void SubSyzorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SubSyzorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SubSyzorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
        
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::MidiMessage m;
    int time;
    
    //Iteration between all midi messages
    for (juce::MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);) {
        if (m.isNoteOn()) {
            int currentNote = m.getNoteNumber();
            bool found = false;
            
            //We first look for either a free oscillator or one that's already playing the same note we want to play next
            for(int j = 0; j < voiceCount && found == false; ++j){
                if(!osc[j].isPlaying() || osc[j].getNote() == currentNote){
                    //osc[j].resetEnv();
                    osc[j].resetFilterEnv();
                    osc[j].setFreq(m.getMidiNoteInHertz(currentNote));
                    osc[j].setNote(currentNote);
                    osc[j].startEnv();
                    osc[j].startFilterEnv();
                    note[j] = m.getMidiNoteInHertz(currentNote);
                    
                    found = true;
                }
            }
            //if there's no free oscillator we look for the one that has been currently playing the longest and overwrite it to the new frequency
            if(!found){
                int oldest = 0;
                for(int k = 0; k < voiceCount; ++k){
                    if(age[k] > age[oldest])
                        oldest = k;
                }
                osc[oldest].setFreq(m.getMidiNoteInHertz(currentNote));
                osc[oldest].setNote(currentNote);
                osc[oldest].startEnv();
                osc[oldest].startFilterEnv();
                note[oldest] = m.getMidiNoteInHertz(currentNote);
                
                age[oldest] = 0;
                
            }
        }
        //If a NoteOff message is received we look for the oscillator which is playing the same note number as the one in the message and we initialize the release sequence of the ADSR
        else if (m.isNoteOff()) {
            int currentNote = m.getNoteNumber();
            bool found = false;
            
            for(int j = 0; j < voiceCount && found == false; ++j){
                if(osc[j].getNote() == currentNote){
                    osc[j].endEnv();
                    osc[j].endFilterEnv();
                    
                    found = true;
                    
                }
            }
        }
    }
    
    int numSamples = buffer.getNumSamples();
    
    //LFO COMPUTATIONS
    if(lfoActive){
        //Compute the value of the lfo for each bufferSize
        lfoAmp = sin(lfoPhase);
        lfoPhase += doublePi*(float)(lfoFreq/currentSampleRate)*(float)numSamples;

        if(lfoPhase > doublePi)
            lfoPhase-=doublePi;
        
        if (lfoSwitch) {
            //Compute the modulated value of both Morph parameters
            //squarenessMod = squareness + ((2*lfoDepth) * lfoAmp);
            squarenessMod = 2 * (1 + lfoDepth * lfoAmp);
            trianglenessMod = triangleness * (1 + ((0.5 + lfoDepth) * lfoAmp));

            DBG(trianglenessMod);
            //Chech if the modulated parameters are inside the possible values
            if (trianglenessMod < 0.0)
                trianglenessMod = 0.0;

            if (squarenessMod < 0.0)
                squarenessMod = 0.0;
            else if (squarenessMod > 4.0)
                squarenessMod = 4.0;
        }
        else {
            // Computation and setting of the new values of frequency to alterate the pitch in each playing osc
            for (int i = 0; i < voiceCount; ++i) {
                osc[i].setFreq(note[i] * pow(2, (lfoDepth * lfoAmp / 6.0)));
            }
        }
    }
    
    //Actually making the Oscillators play notes
    buffer_sum.clear(0, 0, numSamples);

    for (int i = 0; i < voiceCount; ++i){
        //Choosing the behaviour if the LFO is active
        if(lfoActive){
            osc[i].setSquareness(squarenessMod);
            osc[i].setTriangleness(trianglenessMod);
        }
        else{
            osc[i].setSquareness(squareness);
            osc[i].setTriangleness(triangleness);
        }
        //We write to a mono buffer, sum to another temporary buffer and then copy that into the output buffer. Also we reset the "age" parameter of the oscillator while we are checking of it's playing
        if(osc[i].isPlaying()){
            buffer_temp = osc[i].generateBuffer(numSamples); //wave synthesis
            osc[i].processFilter(buffer_temp); //single voice filtering
            age[i] += 1;
            buffer_sum.addFrom(0, 0, buffer_temp, 0, 0, numSamples);
        }
        else
            age[i] = 0;
    }
        
    buffer.copyFrom(0, 0, buffer_sum, 0, 0, numSamples);
    buffer.copyFrom(1, 0, buffer_sum, 0, 0, numSamples);
    
    //Master Output volume and phaser
    juce::dsp::AudioBlock<float> block(buffer);
    
    if(phazorActive)
        phazor.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    masterVolume.process(juce::dsp::ProcessContextReplacing<float>(block));

}

//==============================================================================
bool SubSyzorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SubSyzorAudioProcessor::createEditor()
{
    return new SubSyzorAudioProcessorEditor (*this);
}

//==============================================================================
void SubSyzorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SubSyzorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SubSyzorAudioProcessor();
}

void SubSyzorAudioProcessor::setSquareness(int val){
    squareness = val;
}

void SubSyzorAudioProcessor::setTriangleness(int val){
    triangleness = val;
}

void SubSyzorAudioProcessor::setSquareGain(float val){
    squareGain = val;
}

void SubSyzorAudioProcessor::setTriangleGain(float val){
    triangleGain = val;
}
