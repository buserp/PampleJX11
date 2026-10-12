/*
  ==============================================================================

    Voice.h
    Created: 8 Feb 2026 5:02:26pm
    Author:  Paul Buser

  ==============================================================================
*/

#pragma once

#include "Envelope.h"
#include "Oscillator.h"

struct Voice
{
    int note;
    float saw;
    Oscillator osc1;
    Oscillator osc2;
    Envelope env;

    void reset()
    {
        note = -1;
        saw = 0.0f;
        osc1.reset();
        osc2.reset();
        env.reset();
    }

    float render(float input)
    {
        float sample1 = osc1.nextSample();
        float sample2 = osc2.nextSample();
        saw = saw * 0.997f + sample1 - sample2; // Leaky integrator

        float output = saw + input;
        float envelope = env.nextValue();
        return output * envelope;
    }

    void release()
    {
        env.release();
    }
};
