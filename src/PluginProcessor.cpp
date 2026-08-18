#include "PluginProcessor.h"
#include "PluginEditor.h"

PianoSamplerAudioProcessor::PianoSamplerAudioProcessor()
{
    formatManager.registerBasicFormats();

    // Add voices
    const int maxVoices = 64;
    for (int i = 0; i < maxVoices; ++i)
        synth.addVoice (new juce::SamplerVoice());
}

PianoSamplerAudioProcessor::~PianoSamplerAudioProcessor() {}

void PianoSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
}

void PianoSamplerAudioProcessor::releaseResources() {}

void PianoSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

juce::AudioProcessorEditor* PianoSamplerAudioProcessor::createEditor()
{
    return new PianoSamplerAudioProcessorEditor (*this);
}

bool PianoSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // stereo output only
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

bool PianoSamplerAudioProcessor::loadSampleDirectory (const juce::File& dir)
{
    if (!dir.exists() || !dir.isDirectory()) return false;

    auto manifestFile = dir.getChildFile ("manifest.json");
    if (manifestFile.existsAsFile())
    {
        if (sampleManager.loadManifest (manifestFile, synth, formatManager))
            return true;
        // fall through to naive loader if manifest failed
    }

    auto files = dir.findChildFiles (juce::File::findFiles, false, "*.wav");
    if (files.size() == 0)
        return false;

    // Clear existing sounds
    synth.clearSounds();

    for (auto& f : files)
    {
        juce::AudioFormatReader* reader = formatManager.createReaderFor (f);
        if (reader == nullptr) continue;

        std::unique_ptr<juce::AudioBuffer<float>> buffer (new juce::AudioBuffer<float> ((int)reader->numChannels, (int)reader->lengthInSamples));
        reader->read (buffer->getArrayOfWritePointers(), (int)reader->numChannels, 0, (int)reader->lengthInSamples);

        int rootMidi = sampleManager.inferMidiFromFileName (f.getFileNameWithoutExtension());

        juce::BigInteger allNotes;
        allNotes.setRange (0, 128, true);

        double maxLenSec = (double) reader->lengthInSamples / juce::jmax (1.0, reader->sampleRate);

        // Create a SamplerSound that covers all notes; SampleManager can be extended to create ranges & velocity bands
        auto* sound = new juce::SamplerSound (f.getFileNameWithoutExtension().toStdString(),
                                              *buffer,
                                              allNotes,
                                              rootMidi,
                                              0.0, // attack
                                              0.0, // release
                                              maxLenSec);

        synth.addSound (sound);
        delete reader;
    }

    return true;
}
