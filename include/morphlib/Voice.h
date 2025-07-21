#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

/*
This class wraps MPE logic common to all 
MPE synth plugins. 
*/
struct MTSClient;
namespace morph {
class Synthesiser;
class Voice : public juce::MPESynthesiserVoice 
{
public:
    float getTimbre();
    float getPressure();
    void setGlobalPitchWheel (float normalizedPitchWheel, MTSClient* mtsClient = nullptr);
    void setPitchBendRange (float rangeSemitones);
    void setPitchWheel (float normalizedPitchWheel, MTSClient* mtsClient = nullptr);  // provide a client for non-standard tunings!

    virtual void panic() {}
    virtual void prepareToPlay (double sr, int blockSize) { juce::ignoreUnused (sr, blockSize); }
    virtual void resetAudio() {}
    virtual void allocate (int maxBlockSize) { onAllocation (maxBlockSize); }
    virtual void setSampleRate (double sr) { juce::ignoreUnused (sr); }

    void setPitchWheelNormalized( float pitchWheelNormalized );
    
    virtual void onAllocation (int maxBlockSize) = 0;
    virtual void onNoteStart() = 0;
    virtual void onNoteStop (bool allowTailoff) = 0;
    virtual void onNotePitchbendChanged() = 0;
    virtual void onNotePressureChanged() = 0;
    virtual void onNoteTimbreChanged() = 0;
    virtual void onNoteKeyStateChanged() = 0;
    virtual void onPitchWheelChanged() {};
protected: 
    float pressure {0.0f};
    float timbre {0.0f};
    
    float currentPitchWheel {0.0f};
    float currentPitchWheelNormalized {0.0f};
    double globalPitchBendSemitones {0.0f};
    double adjustedFrequency {0.0f};
    float pitchBendRange {2.0f};
    double initialNote {0.0f};
private:
    
    void noteStarted() override;
    virtual void noteStopped (bool allowTailOff) override;
    virtual void notePitchbendChanged() override;
    virtual void notePressureChanged() override;
    virtual void noteTimbreChanged() override;
    virtual void noteKeyStateChanged() override;
    
    double getGlobalPitchBendSemitones (float normalizedPitchWhel, double bendRangeSemitones);
    double getPitchBendToSemitones (float normalizedPitchWheel);
    double semitonesToScalar (double semitones);
};

}