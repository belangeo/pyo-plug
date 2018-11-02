
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
PyoPluginAudioProcessorEditor::PyoPluginAudioProcessorEditor (PyoPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p) 
{
    fileComp.reset(new FilenameComponent("fileComp",
                                         File("~"),                // current file
                                         false,                    // can edit file name,
                                         false,                    // is directory,
                                         false,                    // is for saving,
                                         {},                       // browser wildcard suffix,
                                         {},                       // enforced suffix,
                                         "Select file to open"));  // text when nothing selected
    fileComp->addListener(this);
    addAndMakeVisible(fileComp.get());

    computeButton.setButtonText("compute");
    computeButton.addListener(this);
    addAndMakeVisible(&computeButton);

    editor.reset(new CodeEditorComponent(codeDocument, &tokeniser));
    editor->loadContent(initialDSP);
    editor->setTabSize(4, true);
    editor->setLineNumbersShown(true);
    editor->setFont(editor->getFont().withHeight(editor->getFont().getHeight() + 4));
    editor->setColour(CodeEditorComponent::backgroundColourId, Colours::white);
    editor->setColour(CodeEditorComponent::lineNumberTextId, Colours::black);
    addAndMakeVisible(editor.get());

    setSize (800, 600);

    computeButton.triggerClick();
}

PyoPluginAudioProcessorEditor::~PyoPluginAudioProcessorEditor() {}

//==============================================================================
void PyoPluginAudioProcessorEditor::paint(Graphics& g) {
    g.fillAll(Colours::lightgrey);
}

void PyoPluginAudioProcessorEditor::resized() {
    fileComp->setBounds(8, 2, (getWidth()-16)/2, 24);
    computeButton.setBounds((getWidth()-16)/2+8, 2, (getWidth()-16)/2, 24);
    editor->setBounds(8, 30, getWidth()-16, getHeight()-36);
}

void PyoPluginAudioProcessorEditor::readFile(const File& fileToRead) {
    if (! fileToRead.existsAsFile())
        return;

    auto fileText = fileToRead.loadFileAsString();
    editor->loadContent(fileText);
}

void PyoPluginAudioProcessorEditor::filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged) {
    if (fileComponentThatHasChanged == fileComp.get())
        readFile(fileComp->getCurrentFile());
}

void PyoPluginAudioProcessorEditor::buttonClicked (Button* button) {
    processor.pyo.clear();
    processor.currentCode = editor->getDocument().getAllContent();
    processor.pyo.exec(processor.currentCode);
}
