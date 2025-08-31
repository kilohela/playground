#include "music.h"
#include "config.h"
#include <cstdint>
#include <cassert>
#include <cctype>

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

void Music::load_midi(const char* filename) {
    // Future work: Implement MIDI file loading
    (void)filename; // suppress unused parameter warning
    assert(false && "MIDI loading not implemented yet");
}

int Music::add_channel(Waveform timbre, float volumn) {
    channels_.push_back({timbre, volumn});
    return channels_.size() - 1;
}

void Music::note_on(int channel, const char* note_name, float velocity) {
    assert(channels_.size() > channel);
    Waveform timbre = channels_[channel].first;
    float volumn = channels_[channel].second;
    uint8_t pitch = note_to_pitch(note_name);
    assert(velocity > 0);
    uint32_t time_in_ticks = beats_to_ticks(time_);
    notes_.push({time_in_ticks, timbre, true, pitch, velocity * volumn});
}

void Music::note_off(int channel, const char* note_name) {
    assert(channels_.size() > channel);
    Waveform timbre = channels_[channel].first;
    uint8_t pitch = note_to_pitch(note_name);
    uint32_t time_in_ticks = beats_to_ticks(time_);
    notes_.push({time_in_ticks, timbre, false, pitch, 0.0f});
}

uint32_t Music::beats_to_ticks(float beats) const {
    return (60.0f / bpm_) * beats * kSampleRate;
}

// start from C0 = MIDI 12
uint8_t Music::note_to_pitch(const char* note) const {
    assert(note != nullptr);
    assert(isalpha(note[0]) && "Invalid note format");
    constexpr int note_offsets[] = {9, 11, 0, 2, 4, 5, 7}; // A, B, C, D, E, F, G
    uint8_t pitch = 60 + (note_offsets[note[0] - 'A']);
    if (note[1] == '#') {
        pitch += 1;
        note += 2;
    } else if (note[1] == 'b') {
        pitch -= 1;
        note += 2;
    } else note += 1;
    assert(isdigit(note[0]) && "Invalid note format");
    pitch += (note[0] - '4') * 12;
    return pitch;
}

} // namespace synth
