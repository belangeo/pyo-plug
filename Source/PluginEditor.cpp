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

const char *initialDSP =
"input = Input([0, 1]).out()\n"
"\n"
"# These LFOs modulate the `freq`, `spread` and `q` arguments of\n"
"# the Phaser object. We give a list of two frequencies in order\n"
"# to create two-streams LFOs, therefore a stereo phasing effect.\n"
"lf1 = Sine(freq=[.1, .15], mul=100, add=250)\n"
"lf2 = Sine(freq=[.18, .13], mul=.4, add=1.5)\n"
"lf3 = Sine(freq=[.07, .09], mul=5, add=6)\n"
"\n"
"# Apply the phasing effect with 20 notches.\n"
"b = Phaser(input, freq=lf1, spread=lf2, q=lf3, num=20, mul=.5).out()\n";

//==============================================================================
PyoPlugAudioProcessorEditor::PyoPlugAudioProcessorEditor (PyoPlugAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p) 
{
    currentFile = String();

    buttonSetup(&newButton, "New");
    buttonSetup(&openButton, "Open");
    buttonSetup(&saveButton, "Save");
    buttonSetup(&saveAsButton, "Save as");
    buttonSetup(&computeButton, "Compute");
    buttonSetup(&zoomOutButton, "-");
    buttonSetup(&zoomInButton, "+");

    editor.reset(new CodeEditorComponent(codeDocument, &tokeniser));
    editor->loadContent(initialDSP);
    editor->setTabSize(4, true);
    editor->setLineNumbersShown(true);
    editor->setFont(editor->getFont().withHeight(editor->getFont().getHeight() + 4));
    editor->setWantsKeyboardFocus(true);
    editor->setColour(CodeEditorComponent::backgroundColourId, Colours::white);
    editor->setColour(CodeEditorComponent::lineNumberTextId, Colours::black);
    addAndMakeVisible(editor.get());

    setSize (900, 700);

    computeButton.triggerClick();
}

PyoPlugAudioProcessorEditor::~PyoPlugAudioProcessorEditor() {}

//==============================================================================
void PyoPlugAudioProcessorEditor::paint(Graphics& g) {
    g.fillAll(Colours::lightgrey);
}

void PyoPlugAudioProcessorEditor::resized() {
    int width = (getWidth() - 16 - 60) / 5;
    newButton.setBounds(8, 2, width, 24);
    openButton.setBounds(8 + width, 2, width, 24);
    saveButton.setBounds(8 + width * 2, 2, width, 24);
    saveAsButton.setBounds(8 + width * 3, 2, width, 24);
    computeButton.setBounds(8 + width * 4, 2, width, 24);
    zoomOutButton.setBounds(8 + width * 5, 2, 30, 24);
    zoomInButton.setBounds(8 + width * 5 + 30, 2, 30, 24);
    editor->setBounds(8, 30, getWidth()-16, getHeight()-36);
}

void PyoPlugAudioProcessorEditor::buttonClicked (Button* button) {
    if (button == &newButton) {
        editor->loadContent("input = Input([0, 1])\n\ninput.out()\n");
        currentFile = String();
    } else if (button == &openButton) {
        FileChooser fc ("Choose a file to open...", File("~"), "", false, false);
        if (fc.browseForFileToOpen()) {
            File chosen = fc.getResults().getReference(0);
            readFile(chosen);
        }
    } else if (button == &saveButton) {
        if (currentFile.isEmpty()) {
            saveAsButton.triggerClick();
        } else {
            File f (currentFile);
            f.replaceWithText(editor->getDocument().getAllContent());
        }
    } else if (button == &saveAsButton) {
        FileChooser fc ("Choose a file to save...", File("~"), "", false, false);
        if (fc.browseForFileToSave(true)) {
            File chosen = fc.getResults().getReference(0);
            chosen.replaceWithText(editor->getDocument().getAllContent());
        }
    } else if (button == &zoomOutButton) {
        editor->setFont(editor->getFont().withHeight(editor->getFont().getHeight() - 1));
    } else if (button == &zoomInButton) {
        editor->setFont(editor->getFont().withHeight(editor->getFont().getHeight() + 1));
    } else if (button == &computeButton) {
        processor.pyo.clear();
        processor.currentCode = editor->getDocument().getAllContent();
        processor.pyo.exec(processor.currentCode);
    }
}

//==============================================================================
void PyoPlugAudioProcessorEditor::readFile(const File& fileToRead) {
    if (! fileToRead.existsAsFile())
        return;
    auto fileText = fileToRead.loadFileAsString();
    editor->loadContent(fileText);
    currentFile = fileToRead.getFullPathName();
}

void PyoPlugAudioProcessorEditor::buttonSetup(TextButton *button, String buttonText) {
    button->setButtonText(buttonText);
    button->addListener(this);
    addAndMakeVisible(button);
}
