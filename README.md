# EchoSphere Delay VST

A versatile stereo delay plugin built using JUCE.

![EchoSphere Screenshot](Resources/screenshot_placeholder.jpg)

## Project Overview

EchoSphere is a professional-grade delay VST/AU plugin designed for music producers, mixing engineers, and sound designers. It provides a clean, intuitive interface with powerful delay capabilities.

### Current Features (v0.1.2 - MVP+)

- Basic delay engine with tempo synchronization
- Ultra-short to long delay times (0.1ms to 2000ms) with logarithmic control
- Feedback control (0% to 100%)
- Wet/dry mix control
- Tempo sync with multiple note values (whole notes to 64th notes, including dotted and triplet)
- Preset saving/loading
- Cross-platform compatibility (VST3, AU)
- Enhanced stability and error handling:
  - Robust against host DAW initialization variations
  - Automatic recovery from potential crash conditions
  - Compatibility with Ableton Live and other major DAWs

## User Guide

### Installation

1. Download the latest release for your operating system
2. Copy the plugin file to your DAW's plugin directory:
   - VST3: `[User]/Documents/VST3/` (Windows) or `/Library/Audio/Plug-Ins/VST3/` (macOS)
   - AU: `/Library/Audio/Plug-Ins/Components/` (macOS)
3. Restart your DAW or rescan plugins

### Basic Usage

1. Insert EchoSphere on an audio track in your DAW
2. Adjust delay time manually using the Time knob, or enable tempo sync
3. Set feedback amount to control the number of repeats
4. Use the mix control to balance between dry and wet signal

## Development

See [DEVELOPMENT.md](DEVELOPMENT.md) for setup instructions and development guidelines.

The project architecture is documented in [ARCHITECTURE.md](ARCHITECTURE.md).

For the development roadmap, see [phases.md](phases.md).

### Requirements

- JUCE framework (version 7.0.0 or newer)
- C++17 compatible compiler
- CMake (version 3.15 or newer)
- macOS: Xcode 12+ or Linux/Windows: Visual Studio 2019+ / gcc 9+

### Building the Project

The project includes scripts for both setup and building.

#### macOS Quick Start

For macOS users, we provide a complete workflow with convenient scripts:

```bash
# Clone the repository
git clone https://github.com/yourusername/echosphere.git
cd echosphere

# Run the setup script (installs all dependencies)
./setup_macos.sh

# Build the plugin
./build.sh
```

#### Build Script Options

The build script supports several options:

```bash
./build.sh --help         # Show help message
./build.sh --clean        # Perform a clean build
./build.sh --debug        # Build debug version
./build.sh --verbose      # Show detailed build output
./build.sh --install      # Build and install to standard plugin directories
```

#### Manual Setup

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
- `setup_macos.sh` - macOS dependency setup script
- `build.sh` - Build and installation script
- `CMakeLists.txt` - CMake configuration
- Documentation:
  - `README.md` - This file
  - `ARCHITECTURE.md` - Technical architecture details
  - `DEVELOPMENT.md` - Development guidelines
  - `phases.md` - Development phases

## License

Copyright (c) 2025 EchoSphere Audio

MIT License - see [LICENSE](LICENSE) for details.

## Acknowledgments

Built with the [JUCE framework](https://juce.com/) 