#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PythonCodeTokeniser.h"

//==============================================================================

class PyoPluginAudioProcessorEditor  : public AudioProcessorEditor,
                                       private TextButton::Listener
{
public:
    PyoPluginAudioProcessorEditor(PyoPluginAudioProcessor&);
    ~PyoPluginAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;
    void buttonClicked(Button* button) override;

    void readFile(const File& fileToRead);

private:
    PyoPluginAudioProcessor& processor;

    PyoPythonTokeniser tokeniser;

    String currentFile;

    TextButton newButton;
    TextButton openButton;
    TextButton saveButton;
    TextButton saveAsButton;
    TextButton computeButton;

    CodeDocument codeDocument;
    std::unique_ptr<CodeEditorComponent> editor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PyoPluginAudioProcessorEditor)
};
