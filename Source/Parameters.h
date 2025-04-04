#pragma once

#include "JuceHeader.h"

namespace EchoSphere
{
    // Parameter IDs - used to identify and access parameters
    namespace ParamIDs
    {
        // Main parameters (Phase 1)
        inline const juce::String DELAY_TIME  = "delay_time";
        inline const juce::String FEEDBACK    = "feedback";
        inline const juce::String MIX         = "mix";
        inline const juce::String SYNC        = "sync";
        inline const juce::String SYNC_NOTE   = "sync_note";
        
        // Additional parameters (future phases)
        inline const juce::String FILTER_FREQ_LP = "filter_freq_lp";
        inline const juce::String FILTER_FREQ_HP = "filter_freq_hp";
        inline const juce::String SATURATION     = "saturation";
        inline const juce::String STEREO_MODE    = "stereo_mode";
        inline const juce::String LFO_RATE       = "lfo_rate";
        inline const juce::String LFO_DEPTH      = "lfo_depth";
        inline const juce::String LFO_WAVEFORM   = "lfo_waveform";
        inline const juce::String LFO_SYNC       = "lfo_sync";
        inline const juce::String LFO_DEST       = "lfo_destination";
    }

    // Sync note values
    enum SyncNote
    {
        WHOLE = 1,
        HALF = 2,
        QUARTER = 3,
        EIGHTH = 4,
        SIXTEENTH = 5,
        THIRTYSECOND = 6,
        SIXTYFOURTH = 7,
        DOTTED_HALF = 8,
        DOTTED_QUARTER = 9,
        DOTTED_EIGHTH = 10,
        DOTTED_SIXTEENTH = 11,
        TRIPLET_HALF = 12,
        TRIPLET_QUARTER = 13,
        TRIPLET_EIGHTH = 14,
        TRIPLET_SIXTEENTH = 15
    };
    
    // Stereo Modes
    enum StereoMode
    {
        MONO = 1,
        LINKED_STEREO = 2,
        PING_PONG = 3,
        DUAL_DELAY = 4
    };
    
    // LFO Waveforms
    enum LfoWaveform
    {
        SINE = 1,
        TRIANGLE = 2,
        SQUARE = 3,
        SAW_UP = 4,
        SAW_DOWN = 5,
        RANDOM = 6
    };

    // Parameter utility class
    class Parameters
    {
    public:
        // Create the parameter layout used by the processor
        static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
        {
            juce::AudioProcessorValueTreeState::ParameterLayout layout;
            
            // Delay Time: From 2 samples up to 2000ms with logarithmic scaling for fine control
            layout.add(std::make_unique<juce::AudioParameterFloat>(
                ParamIDs::DELAY_TIME,
                "Delay Time",
                juce::NormalisableRange<float>(0.045f, 2000.0f, 0.001f, 0.15f), // 0.045ms = ~2 samples at 44.1kHz
                200.0f,
                juce::String(),
                juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { 
                    // Custom formatting based on value range
                    if (value < 0.1f) {
                        // For extremely small values, show samples (assuming 44.1kHz)
                        int samples = static_cast<int>(value * 44.1f); // Approximate samples at 44.1kHz
                        return juce::String(samples) + " samples";
                    }
                    else if (value < 1.0f)
                        return juce::String(value, 2) + " ms"; 
                    else if (value < 10.0f)
                        return juce::String(value, 1) + " ms";
                    else
                        return juce::String(int(value)) + " ms";
                },
                [](const juce::String& text) { 
                    // Handle both ms and samples input
                    if (text.containsIgnoreCase("sample"))
                        return text.getFloatValue() / 44.1f; // Convert samples back to ms
                    return text.getFloatValue(); 
                }
            ));
            
            // Feedback: 0% to 100%
            layout.add(std::make_unique<juce::AudioParameterFloat>(
                ParamIDs::FEEDBACK,
                "Feedback",
                juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
                30.0f,
                juce::String(),
                juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { return juce::String(value, 1) + "%"; },
                [](const juce::String& text) { return text.getFloatValue(); }
            ));
            
            // Mix: 0% to 100%
            layout.add(std::make_unique<juce::AudioParameterFloat>(
                ParamIDs::MIX,
                "Mix",
                juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
                50.0f,
                juce::String(),
                juce::AudioProcessorParameter::genericParameter,
                [](float value, int) { return juce::String(value, 1) + "%"; },
                [](const juce::String& text) { return text.getFloatValue(); }
            ));
            
            // Sync Toggle
            layout.add(std::make_unique<juce::AudioParameterBool>(
                ParamIDs::SYNC,
                "Sync",
                false
            ));
            
            // Sync Note - Quarter note default (Enum parameter)
            juce::StringArray syncNoteNames = { 
                "1/1", "1/2", "1/4", "1/8", "1/16", "1/32", "1/64", 
                "1/2D", "1/4D", "1/8D", "1/16D", 
                "1/2T", "1/4T", "1/8T", "1/16T" 
            };
            
            layout.add(std::make_unique<juce::AudioParameterChoice>(
                ParamIDs::SYNC_NOTE,
                "Sync Note",
                syncNoteNames,
                2  // Default to quarter note (index 2)
            ));
            
            return layout;
        }
    };
} 