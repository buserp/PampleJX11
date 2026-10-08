/*
  ==============================================================================

    Synth.h
    Created: 8 Feb 2026 5:02:20pm
    Author:  Paul Buser

  ==============================================================================
*/

#pragma once

#include "Voice.h"
#include "NoiseGenerator.h"
#include "Utils.h"

class Synth
{
public:
    Synth();
    
    float noiseMix;
    float envAttack;
    float envDecay;
    float envSustain;
    float envRelease;
    
    void allocateResources(double sampleRate, int samplesPerBlock);
    void deallocateResources();
    void reset();
    void render(float** outputBuffers, int sampleCount);
    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);
    void noteOn(int note, int velocity);
    void noteOff(int note);
    
private:
    float sampleRate;
    Voice voice;
    NoiseGenerator noiseGen;
};
