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
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PythonCodeTokeniser.h"

//==============================================================================
class PyoPlugAudioProcessorEditor  : public AudioProcessorEditor,
                                     private TextButton::Listener
{
public:
    PyoPlugAudioProcessorEditor(PyoPlugAudioProcessor&);
    ~PyoPlugAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;
    void buttonClicked(Button* button) override;

    void templateComboChanged();

    void buttonSetup(Button *button, String textButton);
    void readFile(const File& fileToRead);

private:
    PyoPlugAudioProcessor& processor;

    PyoPythonTokeniser tokeniser;

    String currentFile;

    ComboBox templateCombo;
    TextButton newButton;
    TextButton openButton;
    TextButton saveButton;
    TextButton saveAsButton;
    TextButton computeButton;
    TextButton vmpkButton;
    TextButton zoomOutButton;
    TextButton zoomInButton;

    CodeDocument codeDocument;
    std::unique_ptr<CodeEditorComponent> editor;

    MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PyoPlugAudioProcessorEditor)
};
