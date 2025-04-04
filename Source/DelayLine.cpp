#include "DelayLine.h"

namespace EchoSphere
{
    DelayLine::DelayLine() 
        : feedback(0.3f)
        , mix(0.5f)
        , currentSampleRate(44100.0)
        , delayTimeInSamples(0.0f)
        , lastSample(0.0f)
    {
    }

    DelayLine::~DelayLine()
    {
    }

    void DelayLine::prepare(double sampleRate, int maxDelayTimeMs)
    {
        currentSampleRate = sampleRate;
        
        // Calculate maximum delay in samples
        const int maxDelaySamples = static_cast<int>((maxDelayTimeMs / 1000.0) * sampleRate) + 1;
        
        // Prepare delay line
        delayLine.reset();
        delayLine.setMaximumDelayInSamples(maxDelaySamples);
        
        // Reset internal state
        reset();
    }

    void DelayLine::setDelayTime(float delayTimeMs)
    {
        // Convert milliseconds to samples
        delayTimeInSamples = (delayTimeMs / 1000.0f) * static_cast<float>(currentSampleRate);
    }

    void DelayLine::setFeedback(float feedbackAmount)
    {
        // Clamp feedback to ensure stability
        feedback = juce::jlimit(0.0f, 0.99f, feedbackAmount / 100.0f);
    }

    void DelayLine::setMix(float wetDryMix)
    {
        // Clamp mix to valid range
        mix = juce::jlimit(0.0f, 1.0f, wetDryMix / 100.0f);
    }

    float DelayLine::processSample(float inputSample)
    {
        // Get delayed sample
        float delaySample = delayLine.popSample(0, delayTimeInSamples);
        
        // Calculate output with feedback
        float outputSample = inputSample + (delaySample * feedback);
        
        // Push sample to delay line
        delayLine.pushSample(0, outputSample);
        
        // Store for next iteration
        lastSample = delaySample;
        
        // Calculate mixed output (dry/wet)
        return inputSample * (1.0f - mix) + delaySample * mix;
    }

    void DelayLine::processBlock(juce::AudioBuffer<float>& buffer, int channel)
    {
        // Process each sample in the buffer for the specified channel
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = processSample(channelData[sample]);
        }
    }

    void DelayLine::reset()
    {
        delayLine.reset();
        lastSample = 0.0f;
    }
} 