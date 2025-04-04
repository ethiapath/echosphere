# EchoSphere Delay VST

A versatile stereo delay plugin built using JUCE.

## Project Overview
EchoSphere is a professional-grade delay VST/AU plugin designed for music producers, mixing engineers, and sound designers.

## Development Setup

### Requirements
- JUCE framework (version 7.0.0 or newer)
- C++17 compatible compiler
- CMake (version 3.15 or newer)
- macOS: Xcode 12+ or Linux/Windows: Visual Studio 2019+ / gcc 9+

### Building the Project
1. Clone this repository
2. Initialize and update the JUCE submodule:
   ```
   git submodule update --init --recursive
   ```
3. Build using CMake:
   ```
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

## Project Structure
- `Source/` - Core plugin code
  - `PluginProcessor.h/cpp` - Audio processing logic
  - `PluginEditor.h/cpp` - Plugin UI
  - `DelayLine.h/cpp` - Delay line implementation
  - `Parameters.h` - Parameter definitions
- `Resources/` - UI resources, presets, etc.
- `CMakeLists.txt` - CMake configuration

## Features
The plugin is being developed in phases:
1. Basic delay engine (MVP)
2. Stereo and tone shaping
3. Modulation and character
4. Complete UI and preset management
5. Advanced features
6. Final release

See `phases.md` for detailed development phases. 