/*
  ==============================================================================

    Synth.cpp
    Created: 8 Feb 2026 5:02:20pm
    Author:  Paul Buser

  ==============================================================================
*/

#include "Synth.h"

Synth::Synth()
{
    sampleRate = 44100.0f;
}

void Synth::allocateResources(double sampleRate_, int /*samplesPerBlock*/)
{
    sampleRate = static_cast<float>(sampleRate_);
}

void Synth::deallocateResources()
{
    // Do nothing yet
}

void Synth::reset()
{
    for (Voice& v : voices)
    {
        v.reset();
    }
    noiseGen.reset();
    pitchBend = 1.0f;
    sustainPedalPressed = false;
}

void Synth::render(float** outputBuffers, int sampleCount)
{
    float* outputBufferLeft = outputBuffers[0];
    float* outputBufferRight = outputBuffers[1];

    for (Voice& voice : voices)
    {
        voice.osc1.period = voice.period * pitchBend;
        voice.osc2.period = voice.osc1.period * detune;
    }

    for (int sample = 0; sample < sampleCount; ++sample)
    {
        float noise = noiseGen.nextValue() * noiseMix;

        float outputLeft = 0.0f;
        float outputRight = 0.0f;

        for (Voice& voice : voices)
        {
            if (voice.env.isActive())
            {
                float output = voice.render(noise);
                outputLeft += output * voice.panLeft;
                outputRight += output * voice.panRight;
            }
        }
        if (outputBufferRight != nullptr)
        {
            outputBufferLeft[sample] = outputLeft;
            outputBufferRight[sample] = outputRight;
        }
        else
        {
            outputBufferLeft[sample] = (outputLeft + outputRight) * 0.5f;
        }
    }

    for (Voice& voice : voices)
    {
        if (!voice.env.isActive())
        {
            voice.env.reset();
        }
    }

    protectYourEars(outputBufferLeft, sampleCount);
    protectYourEars(outputBufferRight, sampleCount);
}
void Synth::controlChange(uint8_t data1, uint8_t data2)
{
    switch (data1)
    {
        // Sustain pedal
        case 0x40:
        {
            sustainPedalPressed = (data2 >= 64);
            break;
        }
    }
}

void Synth::midiMessage(uint8_t data0, uint8_t data1, uint8_t data2)
{
    // uint8_t channel = data0 & 0x0F;
    switch (data0 & 0xF0)
    {
        // Note off
        case 0x80:
        {
            noteOff(data1 & 0x7F);
            break;
        }
        // Note on
        case 0x90:
        {
            uint8_t note = data1 & 0x7F;
            uint8_t velo = data2 & 0x7F;
            if (velo > 0)
            {
                noteOn(note, velo);
            }
            else
            {
                noteOff(note);
            }
            break;
        }
        // Pitch bend
        case 0xE0:
        {
            pitchBend = std::exp(-0.000014102f * float(data1 + 128 * data2 - 8192));
            break;
        }
        // Control change (CC)
        case 0XB0:
        {
            controlChange(data1, data2);
            break;
        }
    }
}

void Synth::noteOn(int note, int velocity)
{
    int voiceIndex = 0;
    if (numVoices > 1)
    {
        voiceIndex = findFreeVoice();
    }
    startVoice(voiceIndex, note, velocity);
}

void Synth::startVoice(int v, int note, int velocity)
{
    float period = calcPeriod(v, note);
    Voice& voice = voices[v];

    voice.note = note;
    voice.updatePanning();

    voice.period = period;

    voice.osc1.amplitude = (velocity / 127.0f) * 0.5f;
    voice.osc2.amplitude = voice.osc1.amplitude * oscMix;

    voice.osc2.reset();
    voice.osc1.reset();

    Envelope& env = voice.env;

    env.attackMultiplier = envAttack;
    env.decayMultiplier = envDecay;
    env.sustainLevel = envSustain;
    env.releaseMultiplier = envRelease;
    env.attack();
}

void Synth::noteOff(int note)
{
    for (Voice& voice : voices)
    {
        if (voice.note == note)
        {
            voice.release();
            voice.note = -1;
        }
    }
}

float Synth::calcPeriod(int v, int note) const
{
    float period = tune * std::exp2(-EQUAL_TEMPERAMENT_RATIO * (float(note) + ANALOG_DRIFT * float(v)));
    while (period < MIN_PERIOD_IN_SAMPLES || (period * detune) < MIN_PERIOD_IN_SAMPLES)
    {
        period += period;
    }
    return period;
}
int Synth::findFreeVoice() const
{
    int v = 0;
    float l = 100.0f; // initially louder than any envelope
    for (int i = 0; i < MAX_VOICES; i++)
    {
        const auto& voice = voices[i];
        if (voice.env.level < l && !voice.env.isInAttack())
        {
            l = voice.env.level;
            v = i;
        }
    }
    return v;
}
