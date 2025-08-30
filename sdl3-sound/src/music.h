#pragma once

#include <cstdint>
#include <queue>

#include "oscillator.h"

namespace synth {

struct MidiEvent {
    uint32_t tick; // time in ticks
    Waveform timbre;
    bool on;
    uint8_t pitch; // MIDI note number
    float velocity; // 0.0 to 1.0
};

// for monophonic music
class Music {
public:
    Music(int bpm);
    void note_on(float time, Waveform timbre, uint8_t pitch, float velocity);
    void note_off(float time, Waveform timbre, uint8_t pitch);
    const MidiEvent& get_note() const;
    void pop_note();
    bool has_notes() const;

private:
    uint32_t beats_to_ticks(float beats) const;
    std::queue<MidiEvent> notes_;
    int bpm_; // beats per minute
};

} // namespace synth
