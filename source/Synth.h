/*
  ==============================================================================

    Synth.h
    Created: 8 Feb 2026 5:02:20pm
    Author:  Paul Buser

  ==============================================================================
*/

#pragma once

#include "NoiseGenerator.h"
#include "Utils.h"
#include "Voice.h"

constexpr float EQUAL_TEMPERAMENT_RATIO = 1.0f / 12.0f;
constexpr float VELOCITY_CURVE_CONSTANT = 1.0f / 127.0f;
constexpr float MIN_PERIOD_IN_SAMPLES = 6.0f;
constexpr float ANALOG_DRIFT = 0.002f;

class Synth {
public:
    Synth();

    static constexpr int MAX_VOICES = 8;

    int numVoices;
    juce::LinearSmoothedValue<float> outputLevelSmoother;
    float volumeTrim;
    float noiseMix;
    float envAttack;
    float envDecay;
    float envSustain;
    float envRelease;
    float oscMix;
    float detune;
    float tune;
    float pitchBend;
    bool sustainPedalPressed;
    float velocitySensitivity;
    bool ignoreVelocity;

    void allocateResources(double sampleRate, int samplesPerBlock);

    void deallocateResources();

    void reset();

    void render(float** outputBuffers, int sampleCount);

    void controlChange(uint8_t data1, uint8_t data2);

    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);

    void noteOn(int note, int velocity);

    void startVoice(int v, int note, int velocity);

    void noteOff(int note);

    float calcPeriod(int v, int note) const;

    int findFreeVoice() const;

    void restartMonoVoice(int note, int velocity);

    void shiftQueuedNotes();

    int nextQueuedNote();

private:
    float sampleRate;
    std::array<Voice, MAX_VOICES> voices;
    NoiseGenerator noiseGen;
};