#include "music.h"

namespace synth {

Music::Music(int bpm) {
    bpm_ = bpm;
}

const std::vector<Note>& Music::get_notes() const {
    return notes_;
}

void Music::add_note(int pitch, float beats, float velocity) {
    float duration = beats_to_seconds(beats);
    notes_.push_back({pitch, duration, velocity});
}

float Music::beats_to_seconds(float beats) const {
    return (60.0f / bpm_) * beats;
}

} // namespace synth
