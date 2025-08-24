#include "morphlib/BandPassFilter.h"


using namespace morph;

void BandPassFilter::reset()
{
	lowPassFilterLeft.reset();
	lowPassFilterRight.reset();
	highPassFilterLeft.reset();
	highPassFilterRight.reset();	
}

void BandPassFilter::noteOn() { reset(); }

void BandPassFilter::prepare (double sr, int maxBlockSize) 
{ 
	juce::ignoreUnused (maxBlockSize);

	sampleRate = sr; 
	lowFrequencySmoothed.reset (sampleRate, 0.02f);
	lowFrequencySmoothed.setCurrentAndTargetValue (lowFrequencySmoothed.getCurrentValue());
	highFrequencySmoothed.reset (sampleRate, 0.02f);
	highFrequencySmoothed.setCurrentAndTargetValue (highFrequencySmoothed.getCurrentValue());
}

void BandPassFilter::process (juce::AudioBuffer<float>& buffer)
{
	jassert (buffer.getNumChannels() > 0);
	jassert (sampleRate > 0.0); // did you call prepare?

	auto* l = buffer.getWritePointer (0);
	auto* r = buffer.getNumChannels() > 1 ? buffer.getWritePointer (1) : nullptr;

	const int totalSamples {buffer.getNumSamples()};
	int processed {0};
	while (processed < totalSamples)
	{
		const int blockSize = std::min (16, totalSamples - processed);
		float lf = lowFrequencySmoothed.getCurrentValue();
		float hf = highFrequencySmoothed.getCurrentValue();
		lowPassFilterLeft.coefficients   = juce::dsp::IIR::Coefficients<float>::makeLowPass  (sampleRate, hf);
		lowPassFilterRight.coefficients  = juce::dsp::IIR::Coefficients<float>::makeLowPass  (sampleRate, hf);
		highPassFilterLeft.coefficients  = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, lf);
		highPassFilterRight.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, lf);
		lowFrequencySmoothed.skip (blockSize);
		highFrequencySmoothed.skip (blockSize);

		for (int i = 0; i < blockSize; i++)
		{
			int index = processed + i;
			l[index] =     lowPassFilterLeft.processSample  (highPassFilterLeft.processSample (l[index]));
			if( r != nullptr) {
				r[index] = lowPassFilterRight.processSample (highPassFilterRight.processSample (r[index]));
			}
		}

		processed += blockSize;
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