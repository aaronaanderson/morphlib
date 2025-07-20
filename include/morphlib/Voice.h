#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
namespace morph {

/*
    This class wraps MPE logic common to all 
    MPE synth plugins. 
*/
class Synthesiser;
class Voice : public juce::MPESynthesiserVoice 
{
public:
    explicit Voice( Synthesiser& synthesiser );
    float getTimbre();
    float getPressure();
    void setGlobalPitchWheel (float normalizedPitchWheel);
    void setPitchWheel (float normalizedPitchWheel);

    virtual void prepareToPlay (double sr, int blockSize) {};
    virtual void resetAudio() {};
    virtual void allocate (int maxBlockSize) {};

    void setPitchWheelNormalized( float pitchWheelNormalized );

    virtual void onNoteStart() = 0;
    virtual void onNoteStop (bool allowTailoff) = 0;
    virtual void onNotePitchbendChanged() = 0;
    virtual void onNotePressureChanged() = 0;
    virtual void onNoteTimbreChanged() = 0;
    virtual void onNoteKeyStateChanged() = 0;
protected:
    Synthesiser& synthesiser;

    float pressure {0.0f};
    float timbre {0.0f};

    float currentPitchWheel {0.0f};

    float currentPitchWheelNormalized { 0.0f };
    double globalPitchBendSemitones { 0.0f };

    juce::MPENote initialNote;

private:

    void noteStarted() override;
    virtual void noteStopped (bool allowTailOff) override;
    virtual void notePitchbendChanged() override;
    virtual void notePressureChanged() override;
    virtual void noteTimbreChanged() override;
    virtual void noteKeyStateChanged() override;

};

}