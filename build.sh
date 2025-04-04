#!/bin/bash

# EchoSphere Build Script
# This script builds the EchoSphere Delay VST plugin

# Text formatting
BOLD="\033[1m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
RED="\033[0;31m"
BLUE="\033[0;34m"
RESET="\033[0m"

# Default values
BUILD_TYPE="Release"
CLEAN_BUILD=false
VERBOSE=false
INSTALL=false
DESTINATION=""

# Function to display usage
show_help() {
    echo -e "${BOLD}EchoSphere Delay VST - Build Script${RESET}"
    echo
    echo "Usage: $0 [options]"
    echo
    echo "Options:"
    echo "  -h, --help         Show this help message"
    echo "  -c, --clean        Perform a clean build (removes build directory)"
    echo "  -d, --debug        Build Debug version (default is Release)"
    echo "  -v, --verbose      Verbose output during build"
    echo "  -i, --install      Install the plugin after building"
    echo "  --dest=PATH        Specify custom install location (implies --install)"
    echo
    echo "Examples:"
    echo "  $0                 Build release version"
    echo "  $0 --debug         Build debug version"
    echo "  $0 --clean         Clean and rebuild release version"
    echo "  $0 --install       Build and install to default locations"
    echo "  $0 --dest=\"/Users/you/plugins\" Build and install to custom location"
    echo
}

# Parse command-line arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help)
            show_help
            exit 0
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -i|--install)
            INSTALL=true
            shift
            ;;
        --dest=*)
            DESTINATION="${1#*=}"
            INSTALL=true
            shift
            ;;
        *)
            echo -e "${RED}Unknown option: $1${RESET}"
            show_help
            exit 1
            ;;
    esac
done

echo -e "${BOLD}EchoSphere Delay VST - Build Script${RESET}"

# Check if JUCE is present
if [ ! -d "JUCE" ]; then
    echo -e "${RED}Error: JUCE directory not found!${RESET}"
    echo -e "Please run the setup script first: ${BOLD}./setup_macos.sh${RESET}"
    exit 1
fi

# Clean build if requested
if [ "$CLEAN_BUILD" = true ]; then
    echo -e "\n${YELLOW}Performing clean build...${RESET}"
    if [ -d "build" ]; then
        echo "Removing build directory..."
        rm -rf build
    fi
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo -e "\n${BLUE}Creating build directory...${RESET}"
    mkdir -p build
fi

# Navigate to build directory
echo -e "\n${BLUE}Configuring project...${RESET}"
cd build

# Configure with CMake
echo -e "Running CMake for ${BOLD}${BUILD_TYPE}${RESET} build..."
if [ "$VERBOSE" = true ]; then
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
else
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE .. > /dev/null
fi

if [ $? -ne 0 ]; then
    echo -e "\n${RED}CMake configuration failed!${RESET}"
    exit 1
fi

# Build the project
echo -e "\n${BLUE}Building project...${RESET}"

# Set build options
BUILD_OPTIONS=""
if [ "$VERBOSE" = true ]; then
    BUILD_OPTIONS="--verbose"
fi

# Execute build
cmake --build . --config $BUILD_TYPE $BUILD_OPTIONS

if [ $? -ne 0 ]; then
    echo -e "\n${RED}Build failed!${RESET}"
    exit 1
fi

echo -e "\n${GREEN}Build successful!${RESET}"

# Install if requested
if [ "$INSTALL" = true ]; then
    echo -e "\n${BLUE}Installing plugin...${RESET}"
    
    # Determine installation locations based on OS
    if [ "$(uname)" == "Darwin" ]; then # macOS
        VST3_DEST="${DESTINATION:-$HOME/Library/Audio/Plug-Ins/VST3}"
        AU_DEST="${DESTINATION:-/Library/Audio/Plug-Ins/Components}"
        
        # Make sure directories exist
        mkdir -p "$VST3_DEST"
        
        # We may need sudo for system AU directory
        if [ -z "$DESTINATION" ]; then
            sudo mkdir -p "$AU_DEST"
        else
            mkdir -p "$AU_DEST"
        fi
        
        # Copy built plugins
        if [ -d "EchoSphere_artefacts/$BUILD_TYPE/VST3" ]; then
            echo "Installing VST3 to $VST3_DEST"
            cp -R "EchoSphere_artefacts/$BUILD_TYPE/VST3/EchoSphere.vst3" "$VST3_DEST/"
        fi
        
        if [ -d "EchoSphere_artefacts/$BUILD_TYPE/AU" ]; then
            echo "Installing AU to $AU_DEST"
            if [ -z "$DESTINATION" ]; then
                sudo cp -R "EchoSphere_artefacts/$BUILD_TYPE/AU/EchoSphere.component" "$AU_DEST/"
            else
                cp -R "EchoSphere_artefacts/$BUILD_TYPE/AU/EchoSphere.component" "$AU_DEST/"
            fi
        fi
        
        echo -e "${GREEN}Installation complete!${RESET}"
    else # Linux or other
        echo -e "${YELLOW}Automatic installation not implemented for this OS.${RESET}"
        echo "Plugin binaries can be found in the build/EchoSphere_artefacts directory."
    fi
fi

echo -e "\n${GREEN}${BOLD}EchoSphere build process completed successfully!${RESET}"
if [ "$INSTALL" = false ]; then
    echo -e "\nPlugin files are located in ${BOLD}build/EchoSphere_artefacts/${BUILD_TYPE}${RESET}"
    echo -e "To install, run: ${BOLD}$0 --install${RESET}"
fi 