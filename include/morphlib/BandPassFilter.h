#pragma once 

#include <juce_dsp/juce_dsp.h>

namespace morph
{

class BandPassFilter
{
public:
	void prepare (double sr, int maxBlockSize);
	void process (juce::AudioBuffer<float>& buffer);
	void setLowFrequency (float lowFrequencyHz);
	void setHighFrequency (float highFrequencyHz);

private:
	juce::dsp::IIR::Filter<float> lowPassFilterLeft;
	juce::dsp::IIR::Filter<float> lowPassFilterRight;
	juce::dsp::IIR::Filter<float> highPassFilterLeft;
	juce::dsp::IIR::Filter<float> highPassFilterRight;

	double sampleRate {-1.0};
};

}