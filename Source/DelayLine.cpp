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
        // Initialize the delay line with default parameters and proper ProcessSpec
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = currentSampleRate;
        spec.maximumBlockSize = 512; // Reasonable default
        spec.numChannels = 1;        // We process one channel at a time
        
        // First prepare with proper spec
        delayLine.prepare(spec);
        delayLine.setMaximumDelayInSamples(static_cast<int>(currentSampleRate * 5.0)); // 5 seconds at 44.1kHz
        delayLine.reset();
    }

    DelayLine::~DelayLine()
    {
    }

    void DelayLine::prepare(double sampleRate, int maxDelayTimeMs)
    {
        currentSampleRate = sampleRate;
        
        // Calculate maximum delay in samples
        const int maxDelaySamples = static_cast<int>((maxDelayTimeMs / 1000.0) * sampleRate) + 1;
        
        // Properly prepare the delay line with ProcessSpec
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = 512; // Set a reasonable default block size
        spec.numChannels = 1;        // We process one channel at a time
        
        // Prepare delay line with proper spec
        delayLine.prepare(spec);
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
        // Safety check - ensure the delay line is properly initialized
        if (delayLine.getMaximumDelayInSamples() <= 0)
            return inputSample; // Pass through if not initialized
            
        // Allow extremely small delay times (as low as 2 samples in ms equivalent)
        // For linear interpolation to work properly, set absolute minimum to slightly above 0
        const float absoluteMinDelayTime = 0.01f; // Absolute minimum for safety
        
        // Make sure delay time is valid between minimum and maximum
        float validDelayTime = juce::jmax(absoluteMinDelayTime, 
                                         juce::jmin(delayTimeInSamples, 
                                                   static_cast<float>(delayLine.getMaximumDelayInSamples() - 1)));
        
        // Get delayed sample safely
        float delaySample;
        try {
            delaySample = delayLine.popSample(0, validDelayTime);
        }
        catch (...) {
            // If any exception occurs, reset and pass through
            reset();
            return inputSample;
        }
        
        // Calculate output with feedback
        float outputSample = inputSample + (delaySample * feedback);
        
        // Push sample to delay line safely
        try {
            delayLine.pushSample(0, outputSample);
        }
        catch (...) {
            // If any exception occurs, reset and pass through
            reset();
            return inputSample;
        }
        
        // Store for next iteration
        lastSample = delaySample;
        
        // Calculate mixed output (dry/wet)
        return inputSample * (1.0f - mix) + delaySample * mix;
    }

    void DelayLine::processBlock(juce::AudioBuffer<float>& buffer, int channel)
    {
        // Make sure the channel index is valid
        if (channel < 0 || channel >= buffer.getNumChannels())
            return;
            
        // Process each sample in the buffer for the specified channel
        auto* channelData = buffer.getWritePointer(channel);
        
        if (channelData == nullptr)
            return;
            
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