#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class PyoPythonTokeniser   : public CodeTokeniser
{
public:
    //==============================================================================
    PyoPythonTokeniser();
    ~PyoPythonTokeniser();

    //==============================================================================
    int readNextToken (CodeDocument::Iterator&) override;
    CodeEditorComponent::ColourScheme getDefaultColourScheme() override;

    /** The token values returned by this tokeniser. */
    enum TokenType
    {
        tokenType_error = 0,
        tokenType_comment,
        tokenType_keyword,
        tokenType_operator,
        tokenType_identifier,
        tokenType_integer,
        tokenType_float,
        tokenType_string,
        tokenType_bracket,
        tokenType_punctuation
    };

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PyoPythonTokeniser)
};
