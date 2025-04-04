# EchoSphere Development Guide

This document provides guidance for developers working on the EchoSphere Delay VST plugin.

## Development Environment Setup

### Prerequisites

1. **C++ Development Tools**
   - Modern C++ compiler with C++17 support
     - macOS: Xcode Command Line Tools
     - Windows: Visual Studio 2019+
     - Linux: GCC 9+ or Clang 9+
   - CMake 3.15+

2. **JUCE Framework**
   - This project uses JUCE as a Git submodule
   - If not using the submodule, install JUCE 7.0.0+ separately

3. **Audio Plugin SDK (Optional)**
   - VST3 SDK (included with JUCE)
   - AAX SDK (for Pro Tools support)

### Automated Setup (macOS)

For macOS users, we provide a setup script that installs all required dependencies:

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/echosphere.git
   cd echosphere
   ```

2. Run the setup script:
   ```
   ./setup_macos.sh
   ```

This script will:
- Check for and install Xcode Command Line Tools
- Install Homebrew if it's not present
- Install or update CMake to the required version
- Set up the JUCE submodule
- Create a build directory

After running the script, you can proceed directly to building the plugin.

### Manual Setup

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/echosphere.git
   cd echosphere
   ```

2. Initialize and update the JUCE submodule:
   ```
   git submodule update --init --recursive
   ```

3. Create a build directory and generate project files:
   ```
   mkdir build
   cd build
   cmake ..
   ```

4. Build the plugin:
   ```
   cmake --build . --config Release
   ```

## Building the Project

### Using the Build Script

We provide a build script that simplifies the build process with various options:

```
./build.sh [options]
```

Options:
- `-h, --help`: Show help message
- `-c, --clean`: Perform a clean build (removes build directory)
- `-d, --debug`: Build debug version (default is Release)
- `-v, --verbose`: Verbose output during build
- `-i, --install`: Install the plugin after building
- `--dest=PATH`: Specify custom install location (implies --install)

Examples:
```
./build.sh                 # Build release version
./build.sh --debug         # Build debug version
./build.sh --clean         # Clean and rebuild release version
./build.sh --install       # Build and install to default locations
```

### Manual Build Process

If you prefer not to use the build script:

1. Navigate to the build directory:
   ```
   cd build
   ```

2. Configure with CMake:
   ```
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```

3. Build the project:
   ```
   cmake --build . --config Release
   ```

4. Install the plugin (optional):
   - VST3: Copy built `.vst3` file to VST3 plugin directory
   - AU: Copy built `.component` file to Components directory

## Project Structure

```
EchoSphere/
├── CMakeLists.txt             # CMake build configuration
├── ARCHITECTURE.md            # Architecture documentation
├── DEVELOPMENT.md             # This file
├── LICENSE                    # License information
├── README.md                  # Project overview
├── phases.md                  # Development phases
├── setup_macos.sh             # macOS dependency setup script
├── build.sh                   # Build and installation script
├── JUCE/                      # JUCE submodule
├── Resources/                 # Plugin resources (images, presets)
└── Source/                    # Source code
    ├── DelayLine.cpp          # Delay line implementation
    ├── DelayLine.h            # Delay line interface
    ├── Parameters.h           # Parameter definitions
    ├── PluginEditor.cpp       # UI implementation
    ├── PluginEditor.h         # UI interface
    ├── PluginProcessor.cpp    # Audio processor implementation
    └── PluginProcessor.h      # Audio processor interface
```

## Current Development Status

### Phase 1: MVP (Completed)
- Basic delay engine
- Parameter system
- Tempo synchronization
- Simple UI
- Cross-platform build system

### Ready for Testing
The current implementation is ready for basic testing in host DAWs to verify:
- Audio processing quality
- Parameter automation
- Preset saving/loading
- UI functionality
- Cross-platform compatibility

## Next Steps

### Moving to Phase 2: Stereo and Tone Shaping
- Implement advanced stereo modes (linked stereo, ping-pong)
- Add in-loop tone shaping (low-pass and high-pass filters)
- Enhance UI with additional controls
- Expand parameter system for new features

## Development Guidelines

### Code Style

- Use consistent indentation (4 spaces)
- Follow JUCE naming conventions
- Add comments for complex algorithms
- Keep methods focused and concise

### Git Workflow

- Use feature branches for new development
- Create descriptive commit messages
- Update documentation when adding features
- Run tests before committing

### Testing

Test new features in multiple environments:
- Different DAWs (Ableton Live, Logic Pro, Reaper, etc.)
- Different operating systems (Windows, macOS)
- Different sample rates and buffer sizes
- Different plugin formats (VST3, AU)

## Building for Distribution

For release builds:
```
./build.sh --clean          # Clean build
./build.sh                  # Build release version
```

Or manually:
```
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

Plugin binaries will be generated in the `build/EchoSphere_artefacts/Release` directory. 