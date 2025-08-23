#include "morphlib/BandPassFilter.h"


using namespace morph;

void BandPassFilter::prepare (double sr, int maxBlockSize) 
{ 
	sampleRate = sr; 
	lowFrequencySmoothed.reset (sampleRate, 20.0f);
	highFrequencySmoothed.reset (sampleRate, 20.0f);
}

void BandPassFilter::process (juce::AudioBuffer<float>& buffer)
{
	jassert (buffer.getNumChannels() == 2);
	jassert (sampleRate > 0.0); // did you call prepare?

	auto* l = buffer.getWritePointer (0);
	auto* r = buffer.getWritePointer (1);

	int totalSamples {buffer.getNumSamples()};
	int processed {0};
	while (processed < totalSamples)
	{
		const int blockSize = juce::jmax (16, totalSamples - processed);
		float lf = lowFrequencySmoothed.getNextValue();
		float hf = highFrequencySmoothed.getNextValue();
		lowPassFilterLeft.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, lf);
		lowPassFilterRight.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, lf);
		highPassFilterLeft.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, hf);
		highPassFilterRight.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, hf);
		lowFrequencySmoothed.skip (blockSize);
		highFrequencySmoothed.skip (blockSize);

		for (int i = 0; i < blockSize; i++)
		{
			l[i] = lowPassFilterLeft.processSample (highPassFilterLeft.processSample (l[i]));
			r[i] = lowPassFilterRight.processSample (highPassFilterRight.processSample (r[i]));
		}
	}
}

void BandPassFilter::setLowFrequency (float lowFrequencyHz)
{
	jassert (lowFrequencyHz > 0.0f);
	jassert (lowFrequencyHz < float(sampleRate * 0.5));
	lowFrequencySmoothed.setTargetValue (lowFrequencyHz);
}

void BandPassFilter::setHighFrequency (float highFrequencyHz)
{
	jassert (highFrequencyHz > 0.0f);
	jassert (highFrequencyHz < float(sampleRate * 0.5));
	highFrequencySmoothed.setTargetValue (highFrequencyHz);
}