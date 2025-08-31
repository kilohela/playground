#pragma once

#include <cstdint>
#include <queue>
#include <vector>

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
    void note_on(int channel, const char* note_name, float velocity = 0.8f);
    void note_off(int channel, const char* note_name);
    int add_channel(Waveform timbre, float volumn = 1.0f);
    const MidiEvent& get_note() const;
    void pop_note();
    bool has_notes() const;
    void load_midi(const char* filename); // future work
    float time_ = 0.0f;

private:
    std::vector<std::pair<Waveform, float>> channels_;
    uint8_t note_to_pitch(const char* note) const;
    uint32_t beats_to_ticks(float beats) const;
    std::queue<MidiEvent> notes_;
    int bpm_; // beats per minute
};

} // namespace synth
