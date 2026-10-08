/*
  ==============================================================================

    NoiseGenerator.h
    Created: 8 Feb 2026 5:22:06pm
    Author:  Paul Buser

  ==============================================================================
*/

#pragma once

class NoiseGenerator
{
public:
    void reset() {
        noiseSeed = 22222;
    }
    
    float nextValue() {
        // Generate the next integer pseudorandom number.
        noiseSeed = noiseSeed * 196314165 + 907633515;
        
        // Convert to a signed value.
        int temp = int(noiseSeed >> 7) - 16777216;
        
        // Convert to a floating-point number between -1.0 and 1.0.
        return float(temp) / 16777216.0;
    }
private:
    unsigned int noiseSeed;
};
