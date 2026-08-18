#pragma once

#include <JuceHeader.h>
#include "SampleManager.h"

class PianoSamplerAudioProcessor  : public juce::AudioProcessor
{
public:
    PianoSamplerAudioProcessor();
    ~PianoSamplerAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "ColdPiano"; }

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    // State
    bool loadSampleDirectory (const juce::File& dir);

private:
    juce::Synthesiser synth;
    juce::AudioFormatManager formatManager;
    SampleManager sampleManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoSamplerAudioProcessor)
};
