#pragma once

#include <JuceHeader.h>

// Simple helper that parses filenames to infer MIDI notes and provides a place to extend mapping & velocity logic.
class SampleManager
{
public:
    SampleManager() {}

    // Try to parse a midi note from file base name:
    // Accepts formats like "A0", "C#4", "G3_88", or "note_48_vel_127". Falls back to 60 (C4)
    int inferMidiFromFileName (const juce::String& baseName)
    {
        // Try to find note name (A0..G#8)
        juce::String s = baseName;
        s = s.retainCharacters ("ABCDEFGabcdefg0123456789#b");

        // Search for letter A-G
        for (int i = 0; i < s.length(); ++i)
        {
            juce::String part = s.substring (i);
            juce::String note;
            // match e.g. C#4 or C4 or Db3
            if (part.length() >= 2)
            {
                juce::CharPointer_UTF8 p = part.getCharPointer();
                juce::String pitch;
                pitch += p[0];
                int j = 1;
                if (j < part.length() && (p[j] == '#' || p[j] == 'b')) { pitch += p[j]; ++j; }
                // read octave number
                if (j < part.length() && CharacterFunctions::isDigit (p[j]))
                {
                    pitch += p.substring (j, j+1);
                    return noteNameToMidi (pitch);
                }
            }
        }
        // fallback to MIDI 60
        return 60;
    }

private:
    struct CharacterFunctions
    {
        static bool isDigit (juce_wchar c) noexcept { return c >= '0' && c <= '9'; }
    };

    int noteNameToMidi (const juce::String& noteName)
    {
        // Accept forms like C4, C#4, Db3
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

        int midi = (octave + 1) * 12 + semitone; // MIDI octave 4 -> middle C = 60
        return juce::jlimit (0, 127, midi);
    }
};
