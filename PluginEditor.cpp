#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubSyzorAudioProcessorEditor::SubSyzorAudioProcessorEditor (SubSyzorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //SQUARENESS
    squarenessSlider.setRange(1,21,2);
    squarenessSlider.setSliderStyle(juce::Slider::Rotary);
    squarenessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    squarenessSlider.addListener(this);
    squarenessLabel.setText("Squareness", juce::dontSendNotification);
    
    addAndMakeVisible(squarenessSlider);
    addAndMakeVisible(squarenessLabel);
    
    //TRIANGLENESS
    trianglenessSlider.setRange(1,21,1);
    trianglenessSlider.setSliderStyle(juce::Slider::Rotary);
    trianglenessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    trianglenessSlider.addListener(this);
    trianglenessLabel.setText("Triangleness", juce::dontSendNotification);
    
    addAndMakeVisible(trianglenessSlider);
    addAndMakeVisible(trianglenessLabel);
    
    //SQUARE OSC GAIN
    squareGainSlider.setRange(0.0f,1.0f,0.01f);
    squareGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    squareGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    squareGainSlider.addListener(this);
    squareGainLabel.setText("Square Level", juce::dontSendNotification);
    
    squareGainSlider.setValue(0.5f);
    addAndMakeVisible(squareGainSlider);
    addAndMakeVisible(squareGainLabel);
    
    //TRIANGLE OSC GAIN
    triangleGainSlider.setRange(0.0f,1.0f,0.01f);
    triangleGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    triangleGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    triangleGainSlider.addListener(this);
    triangleGainLabel.setText("Triangle Level", juce::dontSendNotification);
    
    triangleGainSlider.setValue(0.5f);
    addAndMakeVisible(triangleGainSlider);
    addAndMakeVisible(triangleGainLabel);
    
    //GENERAL VOLUME
    volumeSlider.setRange(0.0f,1.0f,0.01f);
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    volumeSlider.addListener(this);
    volumeLabel.setText("volume", juce::dontSendNotification);
    
    volumeSlider.setValue(0.5f);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(volumeLabel);
    
    setSize (1000, 600);
}

SubSyzorAudioProcessorEditor::~SubSyzorAudioProcessorEditor()
{
}

//==============================================================================
void SubSyzorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void SubSyzorAudioProcessorEditor::resized()
{
    squarenessSlider.setBounds(0, 0, 200, 200);
    squarenessLabel.setBounds(0, 200, 200, 50);
    trianglenessSlider.setBounds(0, 250, 200, 200);
    trianglenessLabel.setBounds(0, 450, 200, 50);
    
    squareGainSlider.setBounds(300, 0, 100, 400);
    squareGainLabel.setBounds(300, 120, 100, 50);
    triangleGainSlider.setBounds(400, 0, 100, 400);
    triangleGainLabel.setBounds(400, 120, 100, 50);
    volumeSlider.setBounds(600, 0, 100, 400);
    volumeLabel.setBounds(600, 120, 100, 50);

}

void SubSyzorAudioProcessorEditor::sliderValueChanged(juce::Slider* slider){
    if(slider == &squarenessSlider)
        //SET THE SQUARENESS
        audioProcessor.setSquareness(squarenessSlider.getValue());
    else if (slider == &trianglenessSlider)
        //SET HOW TRIANGULAR THE WAVE WILL BE
        audioProcessor.setTriangleness(trianglenessSlider.getValue());
    
    else if (slider == &squareGainSlider)
        audioProcessor.setSquareGain(squareGainSlider.getValue());
    
    else if (slider == &triangleGainSlider)
        audioProcessor.setTriangleGain(triangleGainSlider.getValue());
    
    else if (slider == &volumeSlider)
        audioProcessor.setVolume(volumeSlider.getValue());
}
