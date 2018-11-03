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
#include "PythonCodeTokeniser.h"

struct PythonTokeniserFunctions
{
    static bool isReservedKeyword (String::CharPointerType token, const int tokenLength) noexcept
    {
        static const char* const keywords2Char[] =
            { "as", "if", "in", "is", "or", "EQ", "FM", nullptr };

        static const char* const keywords3Char[] =
            { "try", "not", "for", "def", "del", "and", 
              "FFT", "Mix", "Osc", "SVF", "Sig", "Pan", "Urn", "Abs", 
              "Cos", "Exp", "Log", "Pow", "Sin", "Tan", "LFO", "Seq", 
              "RMS", "Yin", "Max", "Min", "Map", nullptr };

        static const char* const keywords4Char[] =
            { "with", "pass", "from", "exec", "elif", "else", "None", "True", 
              "Clip", "Gate", "Wrap", "IRFM", "Port", "Tone", "Adsr", "Ceil", 
              "Log2", "Sqrt", "Tanh", "HRTF", "SPan", "Blit", "Sine", "Expr", 
              "Expr", "FToM", "MToF", "MToT", "Snap", "Beat", "Iter", "Trig", 
              "IFFT", nullptr };

        static const char* const keywords5Char[] =
            { "yield", "while", "raise", "print", "False", "break", "class", 
              "PVMix", "Atone", "ButBP", "ButBR", "ButHP", "ButLP", "Reson", "Fader", 
              "SigTo", "Delay", "Disto", "STRev", "Scope", "Cloud", "Count", "Metro", 
              "Timer", "Dummy", "Input", "Noise", "RCOsc", "Mixer", "Randh", "Randi", 
              "Score", "AToDB", "DBToA", "Print", "Scale", "Atan2", "Floor", "Log10", 
              "Round", "SLMap", nullptr };

        static const char* const keywords6Char[] =
            { "return", "lambda", "import", "global", "except", "assert", 
              "Change", "Select", "Thresh", "Expseg", "Linseg", "Chorus", "Delay1", "SDelay", 
              "WGVerb", "Biquad", "MoogLP", "Phaser", "Resonx", "Switch", "Bendin", "Notein", 
              "Lorenz", "Phasor", "SumOsc", "ZCross", "Denorm", "Interp", "Record", "Lookup", 
              "Looper", "Pulsar", "Expand", "Mirror", "Choice", "Xnoise", "PVAnal", "PVGate", 
              "PVMult", "PVVerb", "upsamp", "SLMapQ", "Server", "Stream", nullptr };

        static const char* const keywords7Char[] =
            { "finally", "PVCross", "PVDelay", "PVMorph", "PVShift", "PVSynth", "Between", 
              "Compare", "CvlVerb", "Vectral", "Pattern", "Granule", "OscBank", "OscLoop", 
              "OscTrig", "Pointer", "PeakAmp", "VarPort", "ChenLee", "CrossFM", "Rossler", 
              "LogiMap", "RandDur", "RandInt", "OscSend", "CtlScan", "Midictl", "RawMidi", 
              "Touchin", "Allpass", "Average", "Biquada", "Biquadx", "DCBlock", "Hilbert", 
              "IRPulse", "Vocoder", "Balance", "Degrade", "Counter", "Euclide", "Percent", 
              "TrigEnv", "TrigVal", "example", "rescale", "sndinfo", nullptr };

        static const char* const keywords8Char[] =
            {  "continue", "NextTrig", "TrigFunc", "TrigRand", "Resample", "SampHold", 
               "Allpass2", "FourBand", "SfPlayer", "Selector", "Centroid", "Follower", 
               "Spectrum", "CarToPol", "PolToCar", "Particle", "Pointer2", "TablePut", 
               "TableRec", "CtlScan2", "MidiAdsr", "Convolve", "Freeverb", "Compress", 
               "FastSine", "SineLoop", "SuperSaw", "PVAmpMod", "PVBuffer", "PVFilter", 
               "CosTable", "ExpTable", "LinTable", "LogTable", "NewTable", "SawTable", 
               "SndTable", "WinTable", "SLMapDur", "SLMapMul", "SLMapPan", "downsamp", 
               "floatmap", "hzToMidi", "midiToHz", "savefile", nullptr };

        static const char* const keywordsOther[] =
            { "PinkNoise", "NoteinRec", "TrackHold", "MatrixRec", "Programin", "Particle2", 
              "TableFill", "TableRead", "TableScan", "CallAfter", "TrigBurst", "Follower2", 
              "XnoiseDur", "AllpassWG", "FreqShift", "Waveguide", "BandSplit", "IRAverage", 
              "IRWinSinc", "MultiBand", "PVFreqMod", "NewMatrix", "AtanTable", "DataTable", 
              "HannTable", "HarmTable", "ParaTable", "SincTable", "SLMapFreq", "PyoObject",
              "class_args", "getVersion", "sampsToSec", "secToSamps", "SLMapPhase", "ChebyTable", 
              "CurveTable", "FrameAccum", "FrameDelta", "ControlRec", "NoteinRead", "MidiLinseg", 
              "OscReceive", "InputFader", "TrigChoice", "TrigExpseg", "TrigLinseg", "TrigXnoise", 
              "Harmonizer", "ComplexRes", "Granulator", "TableIndex", "TableMorph", "TableScale", 
              "TableWrite", "BrownNoise", "XnoiseMidi", "PVAddSynth", "PVBufLoops", "PyoGuiControlSlider", 
              "PyoGuiVuMeter", "PyoGuiGrapher", "PyoGuiMultiSlider", "PyoGuiSpectrum", "PyoGuiScope", 
              "PyoGuiSndView", "convertStringToSysEncoding", "distanceToSegment", "getPrecision", 
              "getPyoKeywords", "linToCosCurve", "midiToTranspo", "pa_count_devices", "pa_count_host_apis", 
              "pa_get_default_devices_from_host", "pa_get_default_host_api", "pa_get_default_input", 
              "pa_get_default_output", "pa_get_devices_infos", "pa_get_input_devices", 
              "pa_get_input_max_channels", "pa_get_output_devices", "pa_get_output_max_channels", 
              "pa_get_version", "pa_get_version_text", "pa_list_devices", "pa_list_host_apis", 
              "pm_count_devices", "pm_get_default_input", "pm_get_default_output", "pm_get_input_devices", 
              "pm_get_output_devices", "pm_list_devices", "reducePoints", "savefileFromTable", 
              "serverBooted", "serverCreated", "SfMarkerLooper", "SfMarkerShuffler", "SmoothDelay", 
              "OscDataReceive", "OscDataSend", "OscListReceive", "VoiceManager", "MidiDelAdsr", 
              "AttackDetector", "CentsToTranspo", "Clean_objects", "ControlRead", "TranspoToCents", 
              "TrigRandInt", "TrigTableRec", "TrigXnoiseMidi", "MatrixMorph", "MatrixPointer", 
              "MatrixRecLoop", "CosLogTable", "PadSynthTable", "PartialTable", "SharedTable", 
              "SquareTable", "PVBufTabLoops", "PVTranspose", "PyoObjectBase", "PyoTableObject", 
              "PyoMatrixObject", "PyoPVObject", "MidiListener", "OscListener", "TableStream", nullptr };

        const char* const* k;

        switch (tokenLength)
        {
            case 2:   k = keywords2Char; break;
            case 3:   k = keywords3Char; break;
            case 4:   k = keywords4Char; break;
            case 5:   k = keywords5Char; break;
            case 6:   k = keywords6Char; break;
            case 7:   k = keywords7Char; break;
            case 8:   k = keywords8Char; break;

            default:
                if (tokenLength < 2 || tokenLength > 16)
                    return false;

                k = keywordsOther;
                break;
        }

        for (int i = 0; k[i] != 0; ++i)
            if (token.compare (CharPointer_ASCII (k[i])) == 0)
                return true;

        return false;
    }

    template <typename Iterator>
    static int parseIdentifier (Iterator& source) noexcept
    {
        int tokenLength = 0;
        String::CharPointerType::CharType possibleIdentifier [100];
        String::CharPointerType possible (possibleIdentifier);

        while (CppTokeniserFunctions::isIdentifierBody (source.peekNextChar()))
        {
            const juce_wchar c = source.nextChar();

            if (tokenLength < 20)
                possible.write (c);

            ++tokenLength;
        }

        if (tokenLength > 1 && tokenLength <= 16)
        {
            possible.writeNull();

            if (isReservedKeyword (String::CharPointerType (possibleIdentifier), tokenLength))
                return PyoPythonTokeniser::tokenType_keyword;
        }

        return PyoPythonTokeniser::tokenType_identifier;
    }

    template <typename Iterator>
    static int readNextToken (Iterator& source)
    {
        source.skipWhitespace();

        const juce_wchar firstChar = source.peekNextChar();

        switch (firstChar)
        {
        case 0:
            break;

        case '0':   case '1':   case '2':   case '3':   case '4':
        case '5':   case '6':   case '7':   case '8':   case '9':
        case '.':   case '-':
        {
            int result = CppTokeniserFunctions::parseNumber(source);

            if (result == PyoPythonTokeniser::tokenType_error)
            {
                source.skip();

                if (firstChar == '.' || firstChar == '-')
                    return PyoPythonTokeniser::tokenType_punctuation;
            }
            return result;
        }

        case ',':
        case ';':
        case ':':
            source.skip();
            return PyoPythonTokeniser::tokenType_punctuation;

        case '(':   case ')':
        case '{':   case '}':
        case '[':   case ']':
            source.skip();
            return PyoPythonTokeniser::tokenType_bracket;

        case '\'':
		{
            source.skip();
            if (source.peekNextChar() == '\'')
            {   // triple quote string
                source.skip();
                if (source.peekNextChar() == '\'')
                {
				    source.nextChar();
				    for (;;)
				    {
					    const juce_wchar c1 = source.nextChar();
					    const juce_wchar c2 = source.nextChar();
					    if ((c1 == '\'' && c2 == '\'' && source.peekNextChar() == '\'') || c1 == 0 || c2 == 0)
					    {
						    source.skip();
						    break;
					    }
				    }
				    return PyoPythonTokeniser::tokenType_string;
                }
                else
                    return PyoPythonTokeniser::tokenType_error;
			}
			else
		    {   // single quote string
				source.nextChar();
				for (;;)
				{
					const juce_wchar c = source.nextChar();
					if (c == '\'' || c == 0)
					{
				        break;
					}
				}
				return PyoPythonTokeniser::tokenType_string;
		    }
		}

        case '"':
		{
            source.skip();
            if (source.peekNextChar() == '"')
            {   // triple quote string
                source.skip();
                if (source.peekNextChar() == '"')
                {
				    source.nextChar();
				    for (;;)
				    {
					    const juce_wchar c1 = source.nextChar();
					    const juce_wchar c2 = source.nextChar();
					    if ((c1 == '"' && c2 == '"' && source.peekNextChar() == '"') || c1 == 0 || c2 == 0)
					    {
						    source.skip();
						    break;
					    }
				    }
				    return PyoPythonTokeniser::tokenType_string;
                }
                else
                    return PyoPythonTokeniser::tokenType_error;
			}
			else
		    {   // single quote string
				source.nextChar();
				for (;;)
				{
					const juce_wchar c = source.nextChar();
					if (c == '"' || c == 0)
					{
				        break;
					}
				}
				return PyoPythonTokeniser::tokenType_string;
		    }
		}

        case '+':
            source.skip();
            CppTokeniserFunctions::skipIfNextCharMatches (source, '+', '=');
            return PyoPythonTokeniser::tokenType_operator;

        case '#':
            source.skipToEndOfLine();
            return PyoPythonTokeniser::tokenType_comment;

        case '*':   case '%':
        case '=':   case '!':
            source.skip();
            CppTokeniserFunctions::skipIfNextCharMatches (source, '=');
            return PyoPythonTokeniser::tokenType_operator;

        case '?':
        case '~':
            source.skip();
            return PyoPythonTokeniser::tokenType_operator;

        case '<':   case '>':
        case '|':   case '&':   case '^':
            source.skip();
            CppTokeniserFunctions::skipIfNextCharMatches (source, firstChar);
            CppTokeniserFunctions::skipIfNextCharMatches (source, '=');
            return PyoPythonTokeniser::tokenType_operator;

        default:
            if (CppTokeniserFunctions::isIdentifierStart (firstChar))
                return parseIdentifier (source);

            source.skip();
            break;
        }

        return PyoPythonTokeniser::tokenType_error;
    }
};

//==============================================================================
PyoPythonTokeniser::PyoPythonTokeniser() {}
PyoPythonTokeniser::~PyoPythonTokeniser() {}

int PyoPythonTokeniser::readNextToken (CodeDocument::Iterator& source)
{
    return PythonTokeniserFunctions::readNextToken (source);
}

CodeEditorComponent::ColourScheme PyoPythonTokeniser::getDefaultColourScheme()
{
    static const CodeEditorComponent::ColourScheme::TokenType types[] =
    {
        { "Error",          Colour (0xffcc0000) },
        { "Comment",        Colour (0xff5c5c5c) },
        { "Keyword",        Colour (0xff0000cc) },
        { "Operator",       Colour (0xff000000) },
        { "Identifier",     Colour (0xff000000) },
        { "Integer",        Colour (0xff005500) },
        { "Float",          Colour (0xff005500) },
        { "String",         Colour (0xff990099) },
        { "Bracket",        Colour (0xff000000) },
        { "Punctuation",    Colour (0xff000000) }
    };

    CodeEditorComponent::ColourScheme cs;

    for (auto& t : types)
        cs.set (t.name, Colour (t.colour));

    return cs;
}
