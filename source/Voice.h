/*
  ==============================================================================

    Voice.h
    Created: 8 Feb 2026 5:02:26pm
    Author:  Paul Buser

  ==============================================================================
*/

#pragma once

#include "Oscillator.h"
#include "Envelope.h"

struct Voice {
    int note;
    float saw;
    Oscillator osc;
    Envelope env;
    
    void reset() {
        note = -1;
        saw = 0.0f;
        osc.reset();
        env.reset();
    }
    
    float render(float input) {
        float sample = osc.nextSample();
        float envelope = env.nextValue();
        saw = saw * 0.997f + sample; // Leaky integrator
        return (saw + input) * envelope;
    }
    
    void release()
    {
        env.release();
    }
};
