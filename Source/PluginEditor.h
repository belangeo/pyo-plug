#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PythonCodeTokeniser.h"

//==============================================================================

class PyoPluginAudioProcessorEditor  : public AudioProcessorEditor,
                                       public FilenameComponentListener,
                                       private TextButton::Listener
{
public:
    PyoPluginAudioProcessorEditor(PyoPluginAudioProcessor&);
    ~PyoPluginAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;
    void buttonClicked(Button* button) override;
    void filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged) override;

    void readFile(const File& fileToRead);

    TextButton computeButton;

private:
    PyoPluginAudioProcessor& processor;

    PyoPythonTokeniser tokeniser;

    CodeDocument codeDocument;
    std::unique_ptr<CodeEditorComponent> editor;

    std::unique_ptr<FilenameComponent> fileComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PyoPluginAudioProcessorEditor)
};
