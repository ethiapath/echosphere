cmake_minimum_required(VERSION 3.15)
project(EchoSphere VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE as a submodule (assuming it's in a JUCE subdirectory)
add_subdirectory(JUCE)

# Initialize JUCE
juce_add_plugin(EchoSphere
    VERSION 0.1.0
    COMPANY_NAME "EchoSphereAudio"
    COMPANY_COPYRIGHT "Copyright (c) 2025 EchoSphere Audio"
    COMPANY_WEBSITE "https://echosphere.audio"
    BUNDLE_ID "com.echosphereAudio.echosphere"
    PLUGIN_MANUFACTURER_CODE "Echo"
    PLUGIN_CODE "Esph"
    FORMATS VST3 AU Standalone
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    VST3_CATEGORIES "Fx" "Delay"
    AU_MAIN_TYPE "kAudioUnitType_Effect"
    COPY_PLUGIN_AFTER_BUILD TRUE
    PRODUCT_NAME "EchoSphere"
)

# Define source files
target_sources(EchoSphere
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/DelayLine.cpp
        Source/JuceHeader.h
        Source/Parameters.h
)

# Set include paths
target_include_directories(EchoSphere
    PRIVATE
        Source
        ${CMAKE_CURRENT_BINARY_DIR}
)

# Link JUCE modules
target_link_libraries(EchoSphere
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Enable compiler optimizations for release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(EchoSphere
        PRIVATE
            NDEBUG=1
            JUCE_DISPLAY_SPLASH_SCREEN=0
    )
else()
    target_compile_definitions(EchoSphere
        PRIVATE
            DEBUG=1
            JUCE_DISPLAY_SPLASH_SCREEN=0
    )
endif()

# Binary resources
juce_add_binary_data(EchoSphereResources
    SOURCES
        Resources/logo.png
        Resources/placeholder.txt
        Resources/screenshot_placeholder.jpg
)

target_link_libraries(EchoSphere
    PRIVATE
        EchoSphereResources
) 