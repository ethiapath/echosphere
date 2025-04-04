#pragma once

// This is a custom JUCE header file for the EchoSphere project
// It includes all the JUCE modules we need

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_plugin_client/juce_audio_plugin_client.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

// Include binary data resources if needed
#include "BinaryData.h"

#if defined (JUCE_PROJUCER_VERSION) && JUCE_PROJUCER_VERSION < JUCE_VERSION
 /** If you've hit this error then the version of the Projucer that was used to generate this project is
     older than the version of the JUCE modules being included. To fix this error, re-save your project
     using the latest version of the Projucer or, if you aren't using the Projucer to manage your project,
     remove the JUCE_PROJUCER_VERSION define.
 */
 #error "This project was last saved using an outdated version of the Projucer! Re-save this project with the latest version to fix this error."
#endif

// Using the JUCE namespace makes code cleaner to read
using namespace juce; 