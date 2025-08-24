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
	void noteOn();
	void reset();	

private:
	juce::dsp::IIR::Filter<float> lowPassFilterLeft;
	juce::dsp::IIR::Filter<float> lowPassFilterRight;
	juce::dsp::IIR::Filter<float> highPassFilterLeft;
	juce::dsp::IIR::Filter<float> highPassFilterRight;

	juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> lowFrequencySmoothed;
	juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> highFrequencySmoothed;

	double sampleRate {-1.0};
};

}