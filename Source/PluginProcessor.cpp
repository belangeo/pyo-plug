
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PyoPluginAudioProcessor::PyoPluginAudioProcessor() {
    currentCode = "";
}

PyoPluginAudioProcessor::~PyoPluginAudioProcessor() {}

//==============================================================================
const String PyoPluginAudioProcessor::getName() const {
    return JucePlugin_Name;
}

int PyoPluginAudioProcessor::getNumParameters() {
    return 0;
}

float PyoPluginAudioProcessor::getParameter (int index) {
    return 0.0f;
}

void PyoPluginAudioProcessor::setParameter (int index, float newValue) {}

const String PyoPluginAudioProcessor::getParameterName (int index) {
    return String();
}

const String PyoPluginAudioProcessor::getParameterText (int index) {
    return String();
}

const String PyoPluginAudioProcessor::getInputChannelName (int channelIndex) const {
    return String(channelIndex + 1);
}

const String PyoPluginAudioProcessor::getOutputChannelName (int channelIndex) const {
    return String(channelIndex + 1);
}

bool PyoPluginAudioProcessor::isInputChannelStereoPair (int index) const {
    return true;
}

bool PyoPluginAudioProcessor::isOutputChannelStereoPair (int index) const {
    return true;
}

bool PyoPluginAudioProcessor::acceptsMidi() const {
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PyoPluginAudioProcessor::producesMidi() const {
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PyoPluginAudioProcessor::silenceInProducesSilenceOut() const {
    return false;
}

double PyoPluginAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int PyoPluginAudioProcessor::getNumPrograms() {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PyoPluginAudioProcessor::getCurrentProgram() {
    return 0;
}

void PyoPluginAudioProcessor::setCurrentProgram (int index) {}

const String PyoPluginAudioProcessor::getProgramName (int index) {
    return String();
}

void PyoPluginAudioProcessor::changeProgramName (int index, const String& newName) {}

//==============================================================================
void PyoPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    pyo.setup(getTotalNumOutputChannels(), samplesPerBlock, sampleRate);
    if (currentCode != "") {
        pyo.exec(currentCode);
    }
}

void PyoPluginAudioProcessor::releaseResources() {}

void PyoPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
    pyo.process(buffer);
}

//==============================================================================
bool PyoPluginAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PyoPluginAudioProcessor::createEditor() {
    return new PyoPluginAudioProcessorEditor (*this);
}

//==============================================================================
void PyoPluginAudioProcessor::getStateInformation (MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PyoPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new PyoPluginAudioProcessor();
}
