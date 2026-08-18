cmake_minimum_required(VERSION 3.15)
project(ColdPiano)

# Requires a pre-built JUCE CMake package (JUCE 6+). Set JUCE_DIR or install JUCE via your package manager or add as a subproject.
find_package(JUCE CONFIG REQUIRED)

juce_add_plugin(ColdPiano
    COMPANY_NAME "alexrmac27"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE CPno
    FORMATS VST3
)

target_sources(ColdPiano PRIVATE
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/SampleManager.cpp
)

target_compile_features(ColdPiano PRIVATE cxx_std_17)

target_link_libraries(ColdPiano PRIVATE juce::juce_audio_utils juce::juce_dsp)

# Install / export rules if desired
