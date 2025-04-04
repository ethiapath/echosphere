#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace EchoSphere
{
    EchoSphereAudioProcessor::EchoSphereAudioProcessor()
        : AudioProcessor(BusesProperties()
            .withInput("Input", juce::AudioChannelSet::stereo(), true)
            .withOutput("Output", juce::AudioChannelSet::stereo(), true)
        )
        , parameters(*this, nullptr, "EchoSphereParameters", Parameters::createParameterLayout())
    {
        // Link parameter pointers to the actual parameters
        delayTimeParameter = parameters.getRawParameterValue(ParamIDs::DELAY_TIME);
        feedbackParameter = parameters.getRawParameterValue(ParamIDs::FEEDBACK);
        mixParameter = parameters.getRawParameterValue(ParamIDs::MIX);
        syncParameter = parameters.getRawParameterValue(ParamIDs::SYNC);
        syncNoteParameter = parameters.getRawParameterValue(ParamIDs::SYNC_NOTE);
    }

    EchoSphereAudioProcessor::~EchoSphereAudioProcessor()
    {
    }

    void EchoSphereAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        // Resize the delay line vector if needed
        delayLines.clear();
        delayLines.resize(getTotalNumInputChannels());

        // Initialize all delay lines
        for (auto& delayLine : delayLines)
        {
            delayLine.prepare(sampleRate);
        }

        // Initialize delay parameters
        updateDelayParameters();
    }

    void EchoSphereAudioProcessor::releaseResources()
    {
        // Nothing to release in this simple implementation
    }

    bool EchoSphereAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
    {
        // Only supports mono and stereo configurations
        if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
            && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
            return false;

        // Input and output should match
        if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
            return false;

        return true;
    }

    void EchoSphereAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
    {
        juce::ScopedNoDenormals noDenormals;
        const auto numChannels = buffer.getNumChannels();
        const auto numSamples = buffer.getNumSamples();

        // Clear any output channels that don't have input data
        for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
            buffer.clear(i, 0, numSamples);

        // Update parameters if needed
        updateDelayParameters();

        // Process each channel through its own delay line
        for (int channel = 0; channel < juce::jmin(numChannels, static_cast<int>(delayLines.size())); ++channel)
        {
            delayLines[channel].processBlock(buffer, channel);
        }
    }

    void EchoSphereAudioProcessor::updateDelayParameters()
    {
        // Get current parameter values
        float delayTime = *delayTimeParameter;
        float feedback = *feedbackParameter;
        float mix = *mixParameter;
        bool sync = *syncParameter > 0.5f;
        int syncNoteIndex = static_cast<int>(*syncNoteParameter);

        // If sync is enabled, calculate delay time based on host tempo
        if (sync)
        {
            auto bpm = getPlayHead() != nullptr && getPlayHead()->getCurrentPosition().bpm.hasValue()
                ? *getPlayHead()->getCurrentPosition().bpm
                : 120.0; // Default to 120 BPM if host doesn't provide tempo

            delayTime = calculateSyncedDelayTime(static_cast<float>(bpm), syncNoteIndex);
        }

        // Update all delay lines with current parameters
        for (auto& delayLine : delayLines)
        {
            delayLine.setDelayTime(delayTime);
            delayLine.setFeedback(feedback);
            delayLine.setMix(mix);
        }
    }

    float EchoSphereAudioProcessor::calculateSyncedDelayTime(float bpm, int syncNoteIndex)
    {
        // Calculate note durations based on BPM
        // Quarter note duration in ms = 60000 / BPM
        const float quarterNoteMs = 60000.0f / bpm;

        // Calculate based on the selected note value (using the SyncNote enum)
        float multiplier = 1.0f;

        switch (syncNoteIndex + 1) // Adding 1 to match our enum values (1-based)
        {
            case SyncNote::WHOLE:
                multiplier = 4.0f;
                break;
            case SyncNote::HALF:
                multiplier = 2.0f;
                break;
            case SyncNote::QUARTER:
                multiplier = 1.0f;
                break;
            case SyncNote::EIGHTH:
                multiplier = 0.5f;
                break;
            case SyncNote::SIXTEENTH:
                multiplier = 0.25f;
                break;
            case SyncNote::THIRTYSECOND:
                multiplier = 0.125f;
                break;
            case SyncNote::SIXTYFOURTH:
                multiplier = 0.0625f;
                break;
            case SyncNote::DOTTED_HALF:
                multiplier = 3.0f;
                break;
            case SyncNote::DOTTED_QUARTER:
                multiplier = 1.5f;
                break;
            case SyncNote::DOTTED_EIGHTH:
                multiplier = 0.75f;
                break;
            case SyncNote::DOTTED_SIXTEENTH:
                multiplier = 0.375f;
                break;
            case SyncNote::TRIPLET_HALF:
                multiplier = 2.0f / 3.0f;
                break;
            case SyncNote::TRIPLET_QUARTER:
                multiplier = 1.0f / 3.0f;
                break;
            case SyncNote::TRIPLET_EIGHTH:
                multiplier = 0.5f / 3.0f;
                break;
            case SyncNote::TRIPLET_SIXTEENTH:
                multiplier = 0.25f / 3.0f;
                break;
            default:
                multiplier = 1.0f; // Quarter note by default
                break;
        }

        return quarterNoteMs * multiplier;
    }

    float EchoSphereAudioProcessor::getParameterValue(const juce::String& parameterID)
    {
        if (auto* param = parameters.getRawParameterValue(parameterID))
            return *param;
        return 0.0f;
    }

    juce::AudioProcessorEditor* EchoSphereAudioProcessor::createEditor()
    {
        return new juce::GenericAudioProcessorEditor(*this);
    }

    bool EchoSphereAudioProcessor::hasEditor() const
    {
        return true;
    }

    const juce::String EchoSphereAudioProcessor::getName() const
    {
        return JucePlugin_Name;
    }

    bool EchoSphereAudioProcessor::acceptsMidi() const
    {
        return false;
    }

    bool EchoSphereAudioProcessor::producesMidi() const
    {
        return false;
    }

    bool EchoSphereAudioProcessor::isMidiEffect() const
    {
        return false;
    }

    double EchoSphereAudioProcessor::getTailLengthSeconds() const
    {
        // Return a reasonable tail length based on max delay time and feedback
        // For safety, we'll use a fairly large value to accommodate high feedback settings
        return 5.0;
    }

    int EchoSphereAudioProcessor::getNumPrograms()
    {
        return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                    // so this should be at least 1, even if you're not really implementing programs.
    }

    int EchoSphereAudioProcessor::getCurrentProgram()
    {
        return 0;
    }

    void EchoSphereAudioProcessor::setCurrentProgram(int index)
    {
        // Not implemented yet
    }

    const juce::String EchoSphereAudioProcessor::getProgramName(int index)
    {
        return {};
    }

    void EchoSphereAudioProcessor::changeProgramName(int index, const juce::String& newName)
    {
        // Not implemented yet
    }

    void EchoSphereAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
    {
        // Save parameter state as XML
        auto state = parameters.copyState();
        std::unique_ptr<juce::XmlElement> xml(state.createXml());
        copyXmlToBinary(*xml, destData);
    }

    void EchoSphereAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
    {
        // Restore parameter state from XML
        std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
        
        if (xmlState != nullptr && xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
    }

    // This creates new instances of the plugin
    juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
    {
        return new EchoSphereAudioProcessor();
    }
} 