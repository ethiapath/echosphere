#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace EchoSphere
{
    EchoSphereAudioProcessorEditor::EchoSphereAudioProcessorEditor(EchoSphereAudioProcessor& p)
        : AudioProcessorEditor(&p), processorRef(p)
    {
        // Setup UI components and properties
        setupUIComponents();
        
        // Create parameter attachments
        auto& parameters = processorRef.getParameterTree();
        delayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            parameters, ParamIDs::DELAY_TIME, delayTimeSlider);
            
        feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            parameters, ParamIDs::FEEDBACK, feedbackSlider);
            
        mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            parameters, ParamIDs::MIX, mixSlider);
            
        syncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            parameters, ParamIDs::SYNC, syncToggle);
            
        syncNoteAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            parameters, ParamIDs::SYNC_NOTE, syncNoteCombo);
        
        // Set editor size
        setSize(450, 300);
    }

    EchoSphereAudioProcessorEditor::~EchoSphereAudioProcessorEditor()
    {
        // Attachments auto-release
    }

    void EchoSphereAudioProcessorEditor::setupUIComponents()
    {
        // Delay Time Slider
        delayTimeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        delayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
        delayTimeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        delayTimeSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::darkgrey);
        addAndMakeVisible(delayTimeSlider);
        
        delayTimeLabel.setText("Delay Time", juce::dontSendNotification);
        delayTimeLabel.setJustificationType(juce::Justification::centred);
        delayTimeLabel.attachToComponent(&delayTimeSlider, false);
        addAndMakeVisible(delayTimeLabel);
        
        // Feedback Slider
        feedbackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
        feedbackSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        feedbackSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::darkgrey);
        addAndMakeVisible(feedbackSlider);
        
        feedbackLabel.setText("Feedback", juce::dontSendNotification);
        feedbackLabel.setJustificationType(juce::Justification::centred);
        feedbackLabel.attachToComponent(&feedbackSlider, false);
        addAndMakeVisible(feedbackLabel);
        
        // Mix Slider
        mixSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
        mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        mixSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::darkgrey);
        addAndMakeVisible(mixSlider);
        
        mixLabel.setText("Mix", juce::dontSendNotification);
        mixLabel.setJustificationType(juce::Justification::centred);
        mixLabel.attachToComponent(&mixSlider, false);
        addAndMakeVisible(mixLabel);
        
        // Sync Toggle
        syncToggle.setButtonText("Sync to Tempo");
        syncToggle.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
        addAndMakeVisible(syncToggle);
        
        // Sync Note Combo
        syncNoteCombo.addItemList({ "1/1", "1/2", "1/4", "1/8", "1/16", "1/32", "1/64", 
                                    "1/2D", "1/4D", "1/8D", "1/16D", 
                                    "1/2T", "1/4T", "1/8T", "1/16T" }, 1);
        syncNoteCombo.setJustificationType(juce::Justification::centred);
        syncNoteCombo.setSelectedId(3, juce::dontSendNotification); // Quarter note default
        addAndMakeVisible(syncNoteCombo);
        
        syncNoteLabel.setText("Note Value", juce::dontSendNotification);
        syncNoteLabel.setJustificationType(juce::Justification::centred);
        syncNoteLabel.attachToComponent(&syncNoteCombo, false);
        addAndMakeVisible(syncNoteLabel);
    }

    void EchoSphereAudioProcessorEditor::paint(juce::Graphics& g)
    {
        // Fill the background
        g.fillAll(juce::Colour(25, 25, 25));
        
        // Plugin title
        g.setColour(juce::Colours::white);
        g.setFont(24.0f);
        g.drawText("EchoSphere Delay", getLocalBounds(), juce::Justification::centredTop, true);
        
        // Version
        g.setFont(12.0f);
        g.drawText("v0.1.0", getLocalBounds().withTrimmedTop(30), juce::Justification::centredTop, true);
    }

    void EchoSphereAudioProcessorEditor::resized()
    {
        auto area = getLocalBounds().reduced(20);
        auto topArea = area.removeFromTop(80);
        area.removeFromTop(20); // Space between title and controls
        
        // Layout the rotary knobs in a row
        int knobSize = 100;
        int knobAreaWidth = knobSize + 20; // Add some spacing
        
        auto knobArea = area.removeFromTop(knobSize + 40); // Extra height for labels
        
        // Delay Time
        delayTimeSlider.setBounds(knobArea.removeFromLeft(knobAreaWidth).withSizeKeepingCentre(knobSize, knobSize));
        
        // Feedback
        feedbackSlider.setBounds(knobArea.removeFromLeft(knobAreaWidth).withSizeKeepingCentre(knobSize, knobSize));
        
        // Mix
        mixSlider.setBounds(knobArea.removeFromLeft(knobAreaWidth).withSizeKeepingCentre(knobSize, knobSize));
        
        // Bottom row for sync controls
        area.removeFromTop(20); // Space
        auto syncArea = area.removeFromTop(30);
        
        // Sync toggle
        syncToggle.setBounds(syncArea.removeFromLeft(150));
        
        // Note value combo (only visible when sync is enabled)
        syncNoteCombo.setBounds(syncArea.removeFromLeft(100).withTrimmedTop(15));
    }
} 