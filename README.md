# ColdPiano - JUCE Sampler Skeleton using Salamander Grand samples

This project adds a minimal JUCE-based sampler plugin + standalone target that can load a directory of WAVs (for example the Salamander Grand piano sample set) and map them to juce::SamplerSound objects.

Key points
- Expects JUCE (6+) CMake package available (set JUCE_DIR). See https://juce.com/get-juce
- Download Salamander Grand Piano samples (publicly available) and place them in a folder; point the plugin to that folder in the settings UI or via SAMPLE_DIR environment variable.
- This is a skeleton: it loads many single-file SamplerSound objects and uses juce::SamplerVoice to play them. Extend SampleManager to support velocity layers, round-robins, streaming, and note-range mapping.

Build (example)
1. Install JUCE and the VST3 SDK and ensure find_package(JUCE) works.
2. mkdir build && cd build
3. cmake -G "Visual Studio 17 2022" -A x64 ..
4. Open the generated solution or build with msbuild.

Usage
- On first run, point the file browser to the folder containing Salamander WAVs. The plugin will scan and load WAV files that it can parse.

Legal
- Ensure you have the right to distribute the Salamander samples in any builds you ship. Salamander Grand piano is often distributed freely for personal use but check licensing for commercial distribution.
