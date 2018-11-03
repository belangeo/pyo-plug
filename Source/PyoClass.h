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
#ifndef PYOCLASS_H_INCLUDED
#define PYOCLASS_H_INCLUDED

#include "m_pyo.h"
#include "../JuceLibraryCode/JuceHeader.h"

typedef int callPtr(int);

class Pyo {
    public:
        Pyo();
        ~Pyo();
        void setup(int nChannels, int bufferSize, int sampleRate);
        void process(AudioSampleBuffer& buffer);
        void clear();
        int loadfile(const char *file, int add);
        int loadfile(const String &file, int add);
        int exec(const char *msg);
        int exec(const String &msg);
        int value(const char *name, float value);
        int value(const String &name, float value);
        int value(const char *name, float *value, int len);
        int value(const String &name, float *value, int len);
        int set(const char *name, float value);
        int set(const String &name, float value);
        int set(const char *name, float *value, int len);
        int set(const String &name, float *value, int len);
        void midi(int status, int data1, int data2);

    private:
        int nChannels;
        int bufferSize;
        int sampleRate;
        PyThreadState *interpreter;
        float *pyoInBuffer;
        float *pyoOutBuffer;
        callPtr *pyoCallback;
        int pyoId;
        char pyoMsg[262144];
};

#endif  // PYOCLASS_H_INCLUDED
