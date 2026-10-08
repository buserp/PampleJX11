/*
  ==============================================================================

    Envelope.h
    Created: 17 Feb 2026 6:07:05pm
    Author:  Paul Buser

  ==============================================================================
*/

#pragma once

constexpr float SILENCE = 0.0001f;

class Envelope {
  public:
    
    float attackMultiplier;
    float decayMultiplier;
    float sustainLevel;
    float releaseMultiplier;
    
    float level;
    
    float nextValue() {
        level = multiplier * (level - target) + target;
        
        if (level + target > 3.0f)
        {
            multiplier = decayMultiplier;
            target = sustainLevel;
        }
        
        return level;
    }
    
    void reset()
    {
        level = 0.0f;
        target = 0.0f;
        multiplier = 0.0f;
    }
    
    void release()
    {
        target = 0.0f;
        multiplier = releaseMultiplier;
    }
    
    void attack()
    {
        level += SILENCE + SILENCE;
        target = 2.0f;
        multiplier = attackMultiplier;
    }
    
    inline bool isActive() const
    {
        return level > SILENCE;
    }
    
    inline bool isInAttack() const
    {
        return target >= 2.0f;
    }
    
  private:
    float multiplier;
    float target;
};
