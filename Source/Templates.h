#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

const char *StereoDelay =
"input = Input([0, 1]).out()\n"
"\n"
"stdel = Delay(input, delay=[.1, .2], feedback=0.5).out()\n";

const char *StereoVerb =
"input = Input([0, 1]).out()\n"
"\n"
"strev = STRev(input, inpos=[.1, .9], revtime=2, cutoff=5000, bal=0.3).out()\n";

const char *ConvoVerb =
"input = Input([0, 1]).out()\n"
"\n"
"reverb = CvlVerb(input, bal=0.3).out()\n";

const char *MidiSynth =
"class Synth:\n"
"    def __init__(self, transpo=1):\n"
"        # transposition factor.\n"
"        self.transpo = Sig(transpo)\n"
"        # receives midi notes, convert pitch to Hz and manage 10 voices of polyphony.\n"
"        self.note = Notein(poly=10, scale=1, first=0, last=127)\n"
"\n"
"        # handle pitch and velocity (Notein output normalized amplitude (0 -> 1).\n"
"        self.pit = self.note['pitch'] * self.transpo\n"
"        self.amp = MidiAdsr(self.note['velocity'], attack=0.001,\n" 
"                            decay=.1, sustain=.7, release=2, mul=.1)\n"
"\n"
"        # Stereo oscillator, mixed from 10 to 1 stream to avoid alternating channels.\n"
"        self.osc1 = RCOsc(self.pit, sharp=0.5, mul=self.amp).mix(1)\n"
"        self.osc2 = RCOsc(self.pit*0.997, sharp=0.5, mul=self.amp).mix(1)\n"
"\n"
"        # stereo mix\n"
"        self.mix = Mix([self.osc1, self.osc2], voices=2)\n"
"\n"
"    def out(self):\n"
"        'Activates the objet and return self.'\n"
"        self.mix.out()\n"
"        return self\n"
"\n"
"    def sig(self):\n"
"        'Returns the last audio objet for future processing.'\n"
"        return self.mix\n"
"\n"
"synth = Synth()\n"
"\n"
"rev = STRev(synth.sig(), inpos=[0.5], revtime=2, cutoff=4000, bal=.15).out()\n";

StringArray templates ({ nullptr, StereoDelay, StereoVerb, ConvoVerb, MidiSynth });
