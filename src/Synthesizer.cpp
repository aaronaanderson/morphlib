#include "morphlib/Synthesizer.h"

#include "morphlib/Voice.h"

using namespace morph;


juce::Array<Voice*> Synthesizer::getVoices()
{
    juce::Array<Voice*> voiceList;
    for(int i = 0; i < getNumVoices(); i++)
    {
        auto* voice = dynamic_cast<Voice*> (getVoice (i));
        jassert (voice != nullptr);
        voiceList.add (voice);
    }
    return voiceList;     
}

float Synthesizer::getMaximumPressure()
{
    float max = 0.0f;
    for (int i = 0; i < getNumVoices(); i++)
    {
        auto* voice = dynamic_cast<Voice*> (getVoice (i));
        // if (!t->isActive()) break;
        if (voice->getPressure() > max)
            max = voice->getPressure();
    } 
    return max;
}

float Synthesizer::getAverageTimbre()
{
    float sum = 0.0f;
    int numActiveVoices = 0;
    for (int i = 0; i < getNumVoices(); i++)
    {
        auto* voice = dynamic_cast<Voice*> (getVoice (i));
        if (voice->getTimbre() <= 0.0f) break;
        numActiveVoices++;
        sum += voice->getTimbre();
    } 
    if (numActiveVoices == 0) return 0.0f;
    return sum / static_cast<float> (numActiveVoices);
}

void Synthesizer::setGlobalPitchWheel (float pitchWheelNormalized)
{
    for (int i = 0; i < getNumVoices(); i++)
    {
        auto* voice = dynamic_cast<Voice*> (getVoice (i));
        jassert (voice != nullptr);
        voice->setGlobalPitchWheel (pitchWheelNormalized);
    }
}

void Synthesizer::panic()
{
    for (int i = 0; i < getNumVoices(); i++)
    {
        auto* voice = dynamic_cast<Voice*> (getVoice (i));
        jassert (voice != nullptr);
        voice->panic();
    }
}

void Synthesizer::prepareToPlay (double sr, int blockSize)
{
    for (int i = 0; i < getNumVoices(); i++)
    {
        juce::MPESynthesiserVoice* mpev = getVoice (i);
        Voice* v = dynamic_cast<Voice*> (mpev);
        jassert (v != nullptr);
        v->prepareToPlay (sr, blockSize);
    }
    setCurrentPlaybackSampleRate (sr);
}

void Synthesizer::allocate (int maxBlockSize)
{
    for (int i = 0; i < getNumVoices(); i++)
    {
        juce::MPESynthesiserVoice* juceVoice = getVoice (i);
        Voice* v = dynamic_cast<Voice*> (juceVoice);
        jassert (v != nullptr);
        v->allocate (maxBlockSize);        
    }
}

void Synthesizer::handleMidiEvent (const juce::MidiMessage& m)
{
    if (m.isPitchWheel())
    {
        int channel = m.getChannel();
        int pitchWheelValue = m.getPitchWheelValue();
        float normalizedPitchWheel = (pitchWheelValue - 8192) / 8192.0f;
        
        if (channel == 1)
            setGlobalPitchWheel (normalizedPitchWheel);
        else
            setChannelPitchWheel (channel, normalizedPitchWheel);
    }
    juce::MPESynthesiser::handleMidiEvent (m);    
}

void Synthesizer::setChannelPitchWheel (int channel, float normalizedPitchWheel)
{
    for (int i = 0; i < getNumVoices(); i++)
    {
        auto v = getVoice (i);
        auto* voice = dynamic_cast<Voice*> (v);
        if (voice->getCurrentlyPlayingNote().midiChannel == channel)
        {
            voice->setPitchWheel (normalizedPitchWheel);
            break;
        }
    } 
}