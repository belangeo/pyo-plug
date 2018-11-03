
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
    currentFile = String();

    newButton.setButtonText("New");
    newButton.addListener(this);
    addAndMakeVisible(&newButton);

    openButton.setButtonText("Open");
    openButton.addListener(this);
    addAndMakeVisible(&openButton);

    saveButton.setButtonText("Save");
    saveButton.addListener(this);
    addAndMakeVisible(&saveButton);

    saveAsButton.setButtonText("Save as");
    saveAsButton.addListener(this);
    addAndMakeVisible(&saveAsButton);

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
    int width = (getWidth() - 16) / 5;
    newButton.setBounds(8, 2, width, 24);
    openButton.setBounds(8 + width, 2, width, 24);
    saveButton.setBounds(8 + width * 2, 2, width, 24);
    saveAsButton.setBounds(8 + width * 3, 2, width, 24);
    computeButton.setBounds(8 + width * 4, 2, width, 24);
    editor->setBounds(8, 30, getWidth()-16, getHeight()-36);
}

void PyoPluginAudioProcessorEditor::readFile(const File& fileToRead) {
    if (! fileToRead.existsAsFile())
        return;

    auto fileText = fileToRead.loadFileAsString();
    editor->loadContent(fileText);
    currentFile = fileToRead.getFullPathName();
}

void PyoPluginAudioProcessorEditor::buttonClicked (Button* button) {
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
    } else if (button == &computeButton) {
        processor.pyo.clear();
        processor.currentCode = editor->getDocument().getAllContent();
        processor.pyo.exec(processor.currentCode);
    }
}
