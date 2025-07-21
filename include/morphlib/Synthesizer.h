#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
namespace morph {
class Voice;
class Synthesizer : public juce::MPESynthesiser
{
public:
    juce::Array<Voice*> getVoices();
    float getMaximumPressure();
    float getAverageTimbre();
    void setGlobalPitchWheel (float pitchWheelNormalized);
    void panic();
    
    virtual void prepareToPlay (double sr, int blockSize);
    virtual void allocate (int maxBlockSize);

    void handleMidiEvent (const juce::MidiMessage& m) override;
private:
    void setChannelPitchWheel (int channel, float normalizedPitchWheel);
};

}