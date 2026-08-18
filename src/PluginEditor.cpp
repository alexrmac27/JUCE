#include "PluginEditor.h"
#include "PluginProcessor.h"

PianoSamplerAudioProcessorEditor::PianoSamplerAudioProcessorEditor (PianoSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboard (audioProcessor.synth.getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setSize (700, 300);

    addAndMakeVisible (loadButton);
    loadButton.addListener (this);

    addAndMakeVisible (keyboard);
}

PianoSamplerAudioProcessorEditor::~PianoSamplerAudioProcessorEditor() {}

void PianoSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void PianoSamplerAudioProcessorEditor::resized()
{
    auto r = getLocalBounds().reduced (8);
    loadButton.setBounds (r.removeFromTop (30));
    keyboard.setBounds (r.removeFromBottom (130));
}

void PianoSamplerAudioProcessorEditor::buttonClicked (juce::Button* b)
{
    if (b == &loadButton)
    {
        juce::FileChooser chooser ("Select Salamander sample folder...");
        if (chooser.browseForDirectory())
        {
            auto folder = chooser.getResult();
            if (audioProcessor.loadSampleDirectory (folder))
            {
                juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::InfoIcon, "Loaded",
                                                        "Loaded samples from: " + folder.getFullPathName());
            }
            else
            {
                juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::WarningIcon, "Failed",
                                                        "No WAV files found or failed to load.");
            }
        }
    }
}
