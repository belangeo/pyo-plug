/**************************************************************************
 * Copyright 2018 Olivier Belanger                                        *
 *                                                                        *
 * This file is part of pyo-plug, an audio plugin using the python        *
 * module pyo to create the dsp.                                          *
 *                                                                        *
 * pyo-plug is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * pyo-plug is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU LGPL along with pyo-plug.   *
 * If not, see <http://www.gnu.org/licenses/>.                            *
 *                                                                        *
 *************************************************************************/
#include "PluginProcessor.h"
#include "PluginEditor.h"

const char *initialDSP = "input = Input([0, 1])\n\ninput.out()\n";

//==============================================================================
PyoPlugAudioProcessor::PyoPlugAudioProcessor() {
    currentCode = String(initialDSP);
}

PyoPlugAudioProcessor::~PyoPlugAudioProcessor() {}

//==============================================================================
const String PyoPlugAudioProcessor::getName() const {
    return JucePlugin_Name;
}

int PyoPlugAudioProcessor::getNumParameters() {
    return 0;
}

float PyoPlugAudioProcessor::getParameter (int index) {
    return 0.0f;
}

void PyoPlugAudioProcessor::setParameter (int index, float newValue) {}

const String PyoPlugAudioProcessor::getParameterName (int index) {
    return String();
}

const String PyoPlugAudioProcessor::getParameterText (int index) {
    return String();
}

const String PyoPlugAudioProcessor::getInputChannelName (int channelIndex) const {
    return String(channelIndex + 1);
}

const String PyoPlugAudioProcessor::getOutputChannelName (int channelIndex) const {
    return String(channelIndex + 1);
}

bool PyoPlugAudioProcessor::isInputChannelStereoPair (int index) const {
    return true;
}

bool PyoPlugAudioProcessor::isOutputChannelStereoPair (int index) const {
    return true;
}

bool PyoPlugAudioProcessor::acceptsMidi() const {
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PyoPlugAudioProcessor::producesMidi() const {
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PyoPlugAudioProcessor::silenceInProducesSilenceOut() const {
    return false;
}

double PyoPlugAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int PyoPlugAudioProcessor::getNumPrograms() {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PyoPlugAudioProcessor::getCurrentProgram() {
    return 0;
}

void PyoPlugAudioProcessor::setCurrentProgram (int index) {}

const String PyoPlugAudioProcessor::getProgramName (int index) {
    return String();
}

void PyoPlugAudioProcessor::changeProgramName (int index, const String& newName) {}

//==============================================================================
void PyoPlugAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    pyo.setup(getTotalNumOutputChannels(), samplesPerBlock, sampleRate);
    if (currentCode != "") {
        pyo.exec(currentCode);
    }
}

void PyoPlugAudioProcessor::releaseResources() {}

void PyoPlugAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midi) {
    if (midi.getNumEvents() > 0) {
        MidiMessage msg;
        int ignore;
        for (MidiBuffer::Iterator it(midi); it.getNextEvent(msg, ignore); ) {
            unsigned int status = 0, data1 = 0, data2 = 0;
            if (msg.getRawDataSize() > 0) { status = msg.getRawData()[0]; }
            if (msg.getRawDataSize() > 1) { data1 = msg.getRawData()[1]; }
            if (msg.getRawDataSize() > 2) { data2 = msg.getRawData()[2]; }
            pyo.midi(status, data1, data2);
        }
        midi.clear();
    }

    pyo.process(buffer);
}

//==============================================================================
bool PyoPlugAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PyoPlugAudioProcessor::createEditor() {
    return new PyoPlugAudioProcessorEditor (*this);
}

//==============================================================================
void PyoPlugAudioProcessor::getStateInformation (MemoryBlock& destData) {
    String content(currentCode);
    destData.replaceWith((&content)->toUTF8(),
                         CharPointer_UTF8::getBytesRequiredFor(content.getCharPointer()));
}

void PyoPlugAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
    currentCode = String::createStringFromData(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new PyoPlugAudioProcessor();
}
