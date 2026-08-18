#pragma once

#include <JuceHeader.h>

class PianoSamplerAudioProcessor;

class PianoSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                         private juce::Button::Listener
{
public:
    PianoSamplerAudioProcessorEditor (PianoSamplerAudioProcessor&);
    ~PianoSamplerAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PianoSamplerAudioProcessor& audioProcessor;
    juce::TextButton loadButton {"Load Salamander Folder"};
    juce::MidiKeyboardComponent keyboard;

    void buttonClicked (juce::Button* b) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoSamplerAudioProcessorEditor)
};
