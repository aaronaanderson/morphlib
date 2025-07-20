#include "morphlib/Voice.h"

#include "morphlib/MtsEsp.h"

using namespace morph;

// Voice::Voice( Synthesiser& synth )
//     : synthesiser( synth )
// {

// }

float Voice::getPressure() { return pressure; }

float Voice::getTimbre() { return timbre; }

void Voice::setGlobalPitchWheel (float normalizedPitchWheel, MTSClient* mtsClient)
{
    jassert (normalizedPitchWheel >= -1.0f && normalizedPitchWheel <= 1.0f);
    currentPitchWheel = normalizedPitchWheel;
    globalPitchBendSemitones = getGlobalPitchBendSemitones (normalizedPitchWheel, pitchBendRange);
    setPitchWheel (normalizedPitchWheel, mtsClient);
}

void Voice::setPitchWheel (float normalizedPitchWheel, MTSClient* mtsClient) 
{
    jassert (normalizedPitchWheel >= -1.0f && normalizedPitchWheel <= 1.0f);
    currentPitchWheel = normalizedPitchWheel;
    
    double tunedBaseFrequency {0.0f};
    if (mtsClient != nullptr)
    {
        tunedBaseFrequency = MTS_NoteToFrequency (mtsClient, static_cast<char> (initialNote), -1);
    } else {
        tunedBaseFrequency = static_cast<double> (initialNote);
    }
    auto semitones = getPitchBendToSemitones (currentPitchWheel);
    adjustedFrequency = tunedBaseFrequency * semitonesToScalar (semitones + globalPitchBendSemitones);
    onPitchWheelChanged();
}

void Voice::setPitchBendRange (float rangeSemitones) { pitchBendRange = rangeSemitones; }

void Voice::noteStarted()
{
    initialNote = static_cast<double> (getCurrentlyPlayingNote().initialNote);
    onNoteStart();
}

void Voice::noteStopped (bool allowTailoff) { onNoteStop (allowTailoff); }

void Voice::notePitchbendChanged() { onNotePitchbendChanged(); }

void Voice::notePressureChanged()
{
    pressure = getCurrentlyPlayingNote().pressure.asUnsignedFloat();
    onNotePressureChanged();
}

void Voice::noteTimbreChanged()
{
    timbre = getCurrentlyPlayingNote().timbre.asUnsignedFloat();
    onNoteTimbreChanged();
}

void Voice::noteKeyStateChanged()
{
    onNoteKeyStateChanged();
}

double Voice::getGlobalPitchBendSemitones (float normalizedPitchWheel, double bendRange)
{
    return juce::jmap ((double)normalizedPitchWheel, -1.0, 1.0, -bendRange, bendRange);
}

double Voice::getPitchBendToSemitones (float normalizedPitchWheel)
{
    return juce::jmap ((double)normalizedPitchWheel, -1.0, 1.0, -48.0, 48.0);
}

double Voice::semitonesToScalar (double semitones)
{
    return std::pow (2, semitones / 12.0);
}