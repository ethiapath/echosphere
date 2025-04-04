#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

namespace EchoSphere
{
    class EchoSphereAudioProcessorEditor : public juce::AudioProcessorEditor
    {
    public:
        explicit EchoSphereAudioProcessorEditor(EchoSphereAudioProcessor&);
        ~EchoSphereAudioProcessorEditor() override;

        // Component overrides
        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        // Reference to the processor (needed to access parameters)
        EchoSphereAudioProcessor& processorRef;
        
        // UI Components
        juce::Slider delayTimeSlider;
        juce::Slider feedbackSlider;
        juce::Slider mixSlider;
        juce::ToggleButton syncToggle;
        juce::ComboBox syncNoteCombo;
        
        // Labels
        juce::Label delayTimeLabel;
        juce::Label feedbackLabel;
        juce::Label mixLabel;
        juce::Label syncLabel;
        juce::Label syncNoteLabel;
        
        // Parameter attachments (to connect UI with parameters)
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> syncNoteAttachment;
        
        // Setup UI components with initial properties
        void setupUIComponents();
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EchoSphereAudioProcessorEditor)
    };
} 