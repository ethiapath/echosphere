# EchoSphere Architecture

This document describes the architecture of the EchoSphere delay plugin.

## Overview

EchoSphere is built using the JUCE framework, which provides cross-platform audio plugin capabilities. The plugin follows the standard JUCE audio processor architecture with separation between DSP code and UI.

## Core Components

### Parameter Management (`Parameters.h`)

The parameter system defines all plugin parameters and their ranges using JUCE's AudioProcessorValueTreeState system:

- **Parameter IDs**: Unique string identifiers for each parameter
- **Parameter Ranges**: Min/max values, steps, and skew factors
- **Default Values**: Initial parameter values
- **Value Formatting**: Functions to convert between raw values and formatted display text

Parameters are organized into logical groups:
- Core parameters (delay time, feedback, mix)
- Sync parameters (sync toggle, note values)
- Future parameters (filters, modulation, etc.)

### Delay Engine (`DelayLine.h/cpp`)

The delay engine implements the core DSP functionality:

- **Delay Buffer**: Uses JUCE's DelayLine class with linear interpolation
- **Parameter Controls**: Methods to set delay time, feedback, and mix
- **Audio Processing**: Sample-by-sample and block processing methods
- **State Management**: Methods for initialization and reset

The delay algorithm:
1. Retrieves delayed audio from the buffer
2. Applies feedback to the input signal
3. Writes the processed signal to the delay buffer
4. Mixes dry and wet signals based on the mix parameter

### Plugin Processor (`PluginProcessor.h/cpp`)

The processor is the core plugin component that:
- Manages audio I/O and plugin lifecycle
- Creates and configures delay lines
- Handles parameter changes
- Processes audio blocks
- Manages plugin state (preset saving/loading)
- Calculates tempo-synced delay times

Key functionality:
- Multiple delay lines (one per channel)
- Parameter update system
- Tempo synchronization with host DAW

### Plugin Editor (`PluginEditor.h/cpp`)

The UI layer that:
- Creates all control components (knobs, buttons, etc.)
- Links UI controls to parameters
- Handles layout and visual styling
- Responds to user interaction

## Data Flow

1. **Host → Plugin**: Audio samples and parameter automation
2. **Parameter System → Delay Engine**: Parameter changes are applied to the delay lines
3. **Plugin Processor → Delay Engine**: Audio blocks are sent for processing
4. **Delay Engine → Plugin Processor**: Processed audio returned
5. **Plugin Processor → Host**: Final audio output sent to DAW

## Build System

CMake is used for cross-platform builds, configured to:
- Set C++17 standard
- Link required JUCE modules
- Configure plugin metadata
- Define build targets for multiple plugin formats
- Set appropriate compiler flags and optimizations

## Future Expansion

The architecture is designed for modular expansion with:
- Additional DSP modules (filters, modulation)
- Enhanced stereo capabilities
- Extended parameter management
- UI component extensions 