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
    
    struct VoiceListener 
    {
        virtual ~VoiceListener() {}
        virtual void voiceAdded (Voice* newVoice) = 0;
        virtual void voiceRemoved (Voice* voiceRemoved) = 0;
    };
    void addVoiceListener (VoiceListener* vl);
    void removeVoiceListener (VoiceListener* vl);

    void handleMidiEvent (const juce::MidiMessage& m) override;
private:
    juce::Array<VoiceListener*> voiceListeners;
    void setChannelPitchWheel (int channel, float normalizedPitchWheel);
};

}