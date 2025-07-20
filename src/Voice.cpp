#include "morphlib/Voice.h"

#include "morphlib/MtsEsp.h"

using namespace morph;

Voice::Voice( Synthesiser& synth )
    : synthesiser( synth )
{

}

float Voice::getPressure() { return pressure; }

float Voice::getTimbre() { return timbre; }

void Voice::setGlobalPitchWheel (float normalizedPitchWheel)
{
    jassert (normalizedPitchWheel >= -1.0f && normalizedPitchWheel <= 1.0f);
    currentPitchWheel = normalizedPitchWheel;
    auto tunedBaseFrequency = MTS_NoteToFrequency (&mtsClient, static_cast<char> (initialNote), -1);
    auto semitones = getPitchBendToSemitones (pitchWheel);
    auto adjustedFrequency = tunedBaseFrequency * semitonesToScalar (semitones + globalPitchBendSemitones);
    trajectory.setFrequencySmooth (static_cast<float> (adjustedFrequency));    
}

void Voice::setPitchWheelNormalized( float pitchWheelNormalized ) 
{
    currentPitchWheelNormalized = pitchWheelNormalized;

}

void Voice::noteStarted()
{
    initialNote = getCurrentlyPlayingNote();
    onNoteStart();
}

void Voice::noteStopped (bool allowTailoff)
{
    onNoteStop (allowTailoff);
}

void Voice::notePitchbendChanged()
{
    onNotePitchbendChanged();
}

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

void Voice::onNoteKeyStateChanged()
{
    onNoteKeyStateChanged();
}