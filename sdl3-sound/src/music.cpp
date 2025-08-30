#include "music.h"
#include "config.h"
#include <cstdint>
#include <cassert>

namespace synth {

Music::Music(int bpm) {
    bpm_ = bpm;
}

const MidiEvent& Music::get_note() const {
    return notes_.front();
}

bool Music::has_notes() const {
    return !notes_.empty();
}

void Music::pop_note() {
    assert(!notes_.empty());
    notes_.pop();
}

void Music::note_on(float time, Waveform timbre, uint8_t pitch, float velocity) {
    assert(velocity > 0);
    uint32_t time_in_ticks = beats_to_ticks(time);
    notes_.push({time_in_ticks, timbre, true, pitch, velocity});
}

void Music::note_off(float time, Waveform timbre, uint8_t pitch) {
    uint32_t time_in_ticks = beats_to_ticks(time);
    notes_.push({time_in_ticks, timbre, false, pitch, 0.0f});
}

uint32_t Music::beats_to_ticks(float beats) const {
    return (60.0f / bpm_) * beats * kSampleRate;
}

} // namespace synth
