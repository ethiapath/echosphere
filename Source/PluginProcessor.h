#pragma once

#include "JuceHeader.h"
#include "Parameters.h"
#include "DelayLine.h"

namespace EchoSphere
{
    class EchoSphereAudioProcessor : public juce::AudioProcessor
    {
    public:
        EchoSphereAudioProcessor();
        ~EchoSphereAudioProcessor() override;

        // AudioProcessor implementation
        void prepareToPlay(double sampleRate, int samplesPerBlock) override;
        void releaseResources() override;
        
        bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
        
        void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
        
        juce::AudioProcessorEditor* createEditor() override;
        bool hasEditor() const override;
        
        const juce::String getName() const override;
        
        bool acceptsMidi() const override;
        bool producesMidi() const override;
        bool isMidiEffect() const override;
        double getTailLengthSeconds() const override;
        
        int getNumPrograms() override;
        int getCurrentProgram() override;
        void setCurrentProgram(int index) override;
        const juce::String getProgramName(int index) override;
        void changeProgramName(int index, const juce::String& newName) override;
        
        void getStateInformation(juce::MemoryBlock& destData) override;
        void setStateInformation(const void* data, int sizeInBytes) override;

        // Get the parameter tree state (for UI)
        juce::AudioProcessorValueTreeState& getParameterTree() { return parameters; }
        
        // Utility function to get a parameter's raw value
        float getParameterValue(const juce::String& parameterID);
        
        // Convert sync note index to delay time in ms based on host tempo
        float calculateSyncedDelayTime(float bpm, int syncNoteIndex);
        
    private:
        // Parameter handling
        juce::AudioProcessorValueTreeState parameters;
        
        // Delay lines (one per channel for stereo)
        std::vector<DelayLine> delayLines;
        
        // Parameter change listeners
        std::atomic<float>* delayTimeParameter = nullptr;
        std::atomic<float>* feedbackParameter = nullptr;
        std::atomic<float>* mixParameter = nullptr;
        std::atomic<float>* syncParameter = nullptr;
        std::atomic<float>* syncNoteParameter = nullptr;
        
        // Update delay parameters based on the current parameter values
        void updateDelayParameters();
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EchoSphereAudioProcessor)
    };
} 