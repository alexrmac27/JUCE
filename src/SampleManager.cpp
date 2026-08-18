#include "SampleManager.h"
#include <JuceHeader.h>

bool SampleManager::loadManifest (const juce::File& manifestFile, juce::Synthesiser& synth, juce::AudioFormatManager& formatManager)
{
    if (!manifestFile.existsAsFile()) return false;

    juce::String json = manifestFile.loadFileAsString();
    juce::var parsed = juce::JSON::parse(json);
    auto* arr = parsed.getArray();
    if (arr == nullptr) return false;

    juce::File baseDir = manifestFile.getParentDirectory();

    synth.clearSounds();

    for (auto& itemVar : *arr)
    {
        if (! itemVar.isObject()) continue;
        juce::var fileVar = itemVar["file"];
        juce::String filename = fileVar.toString();
        juce::File wavFile = baseDir.getChildFile (filename);
        if (! wavFile.existsAsFile()) continue;

        juce::AudioFormatReader* reader = formatManager.createReaderFor (wavFile);
        if (reader == nullptr) continue;

        std::unique_ptr<juce::AudioBuffer<float>> buffer (new juce::AudioBuffer<float> ((int)reader->numChannels, (int)reader->lengthInSamples));
        reader->read (buffer->getArrayOfWritePointers(), (int)reader->numChannels, 0, (int)reader->lengthInSamples);

        int rootMidi = (int) itemVar["root"]; // fallbacks handled below
        if (rootMidi <= 0 || rootMidi > 127) rootMidi = inferMidiFromFileName (wavFile.getFileNameWithoutExtension());

        int lo = (int) itemVar["lo"];
        int hi = (int) itemVar["hi"];
        if (lo < 0 || lo > 127) lo = rootMidi;
        if (hi < 0 || hi > 127) hi = rootMidi;

        juce::BigInteger notes;
        notes.clear();
        notes.setRange (lo, hi - lo + 1, true);

        double maxLenSec = (double) reader->lengthInSamples / juce::jmax (1.0, reader->sampleRate);

        auto* sound = new juce::SamplerSound (wavFile.getFileNameWithoutExtension().toStdString(),
                                              *buffer,
                                              notes,
                                              rootMidi,
                                              0.01, // small attack
                                              0.5,  // release
                                              maxLenSec);
        synth.addSound (sound);
        delete reader;
    }

    return true;
}


int SampleManager::inferMidiFromFileName (const juce::String& baseName)
{
    // fallback parser similar to header implementation
    juce::String s = baseName;
    s = s.retainCharacters ("ABCDEFGabcdefg0123456789#b");

    for (int i = 0; i < s.length(); ++i)
    {
        juce::String part = s.substring (i);
        if (part.length() >= 2)
        {
            juce::CharPointer_UTF8 p = part.getCharPointer();
            juce::String pitch;
            pitch += p[0];
            int j = 1;
            if (j < part.length() && (p[j] == '#' || p[j] == 'b')) { pitch += p[j]; ++j; }
            if (j < part.length() && CharacterFunctions::isDigit (p[j]))
            {
                pitch += part.substring (j, j+1);
                return noteNameToMidi (pitch);
            }
        }
    }
    return 60;
}

int SampleManager::noteNameToMidi (const juce::String& noteName)
{
    if (noteName.length() < 2) return 60;
    juce::String name = noteName.trim();
    juce::String letter = name.substring (0,1).toUpperCase();
    int semitone = 0;
    if (letter == "C") semitone = 0;
    else if (letter == "D") semitone = 2;
    else if (letter == "E") semitone = 4;
    else if (letter == "F") semitone = 5;
    else if (letter == "G") semitone = 7;
    else if (letter == "A") semitone = 9;
    else if (letter == "B") semitone = 11;

    int idx = 1;
    if (name.length() > 1)
    {
        auto c = name.substring (1,2);
        if (c == "#") { semitone += 1; ++idx; }
        else if (c == "B" || c == "b") { semitone -= 1; ++idx; }
    }

    int octave = 4;
    if (idx < name.length()) octave = name.substring (idx).getIntValue();

    int midi = (octave + 1) * 12 + semitone;
    return juce::jlimit (0, 127, midi);
}
