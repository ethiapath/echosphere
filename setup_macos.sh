#!/bin/bash

# EchoSphere macOS Setup Script
# This script sets up all the dependencies required to build the EchoSphere Delay VST on macOS

# Text formatting
BOLD="\033[1m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
RED="\033[0;31m"
RESET="\033[0m"

echo -e "${BOLD}EchoSphere Delay VST - macOS Setup${RESET}"
echo "This script will set up the necessary dependencies for building EchoSphere."
echo ""

# Check for Xcode Command Line Tools
echo -e "${BOLD}Checking for Xcode Command Line Tools...${RESET}"
if ! xcode-select -p &> /dev/null; then
    echo -e "${YELLOW}Xcode Command Line Tools not found. Installing...${RESET}"
    xcode-select --install
    echo "Please wait for the installation to complete, then run this script again."
    exit 0
else
    echo -e "${GREEN}Xcode Command Line Tools are installed.${RESET}"
fi

# Check for Homebrew
echo -e "\n${BOLD}Checking for Homebrew...${RESET}"
if ! command -v brew &> /dev/null; then
    echo -e "${YELLOW}Homebrew not found. Installing...${RESET}"
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    
    # Add Homebrew to PATH if needed
    if [[ $(uname -m) == "arm64" ]]; then
        echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
        eval "$(/opt/homebrew/bin/brew shellenv)"
    fi
    
    echo -e "${GREEN}Homebrew has been installed.${RESET}"
else
    echo -e "${GREEN}Homebrew is installed.${RESET}"
fi

# Check for CMake
echo -e "\n${BOLD}Checking for CMake...${RESET}"
if ! command -v cmake &> /dev/null; then
    echo -e "${YELLOW}CMake not found. Installing...${RESET}"
    brew install cmake
    echo -e "${GREEN}CMake has been installed.${RESET}"
else
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d" " -f3)
    echo -e "${GREEN}CMake version ${CMAKE_VERSION} is installed.${RESET}"
    
    # Check if CMake version meets requirements
    if [[ $(echo "${CMAKE_VERSION}" | cut -d. -f1) -lt 3 || ($(echo "${CMAKE_VERSION}" | cut -d. -f1) -eq 3 && $(echo "${CMAKE_VERSION}" | cut -d. -f2) -lt 15) ]]; then
        echo -e "${YELLOW}Updating CMake to version 3.15 or higher...${RESET}"
        brew upgrade cmake
        echo -e "${GREEN}CMake has been updated to $(cmake --version | head -n1 | cut -d" " -f3).${RESET}"
    fi
fi

# Set up JUCE submodule
echo -e "\n${BOLD}Setting up JUCE framework...${RESET}"

if [ ! -d "JUCE" ]; then
    echo -e "${YELLOW}JUCE directory not found. Initializing as submodule...${RESET}"
    if [ -d ".git" ]; then
        # This is already a git repository, just add JUCE as a submodule
        git submodule add https://github.com/juce-framework/JUCE.git
    else
        # This isn't a git repository yet, create one and add JUCE
        echo -e "${YELLOW}Initializing git repository...${RESET}"
        git init
        git submodule add https://github.com/juce-framework/JUCE.git
    fi
    echo -e "${GREEN}JUCE submodule has been added.${RESET}"
else
    echo -e "${GREEN}JUCE directory found. Updating submodule...${RESET}"
    git submodule update --init --recursive
fi

# Create build directory and run initial CMake configuration
echo -e "\n${BOLD}Setting up build environment...${RESET}"
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir -p build
fi

echo -e "\n${BOLD}All dependencies are now installed!${RESET}"
echo -e "To build EchoSphere, run:\n"
echo -e "  cd build"
echo -e "  cmake .."
echo -e "  cmake --build ."
echo -e "\nHappy coding!" 