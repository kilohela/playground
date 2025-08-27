#include "music.h"

namespace synth {

Music::Music() {}

const std::vector<Note>& Music::get_notes() const {
    return notes_;
}

void Music::add_note(int pitch, float duration, float velocity) {
    notes_.push_back({pitch, duration, velocity});
}

} // namespace synth
