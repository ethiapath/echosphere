#pragma once

#include <JuceHeader.h>

namespace EchoSphere
{
    class DelayLine
    {
    public:
        DelayLine();
        ~DelayLine();
        
        // Initialize the delay line with sample rate
        void prepare(double sampleRate, int maxDelayTimeMs = 5000);
        
        // Set the delay time in milliseconds
        void setDelayTime(float delayTimeMs);
        
        // Set the feedback amount (0.0 - 1.0)
        void setFeedback(float feedbackAmount);
        
        // Set the wet/dry mix (0.0 - 1.0)
        void setMix(float wetDryMix);
        
        // Process a single sample through the delay
        float processSample(float inputSample);
        
        // Process a block of audio
        void processBlock(juce::AudioBuffer<float>& buffer, int channel);
        
        // Reset the delay line's internal state
        void reset();
        
    private:
        juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
        float feedback;
        float mix;
        double currentSampleRate;
        float delayTimeInSamples;
        float lastSample;
    };
} 