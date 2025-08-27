#pragma once

#include <vector>

namespace synth {

struct Note {
    int pitch; // MIDI note number
    float duration; // in seconds
    float velocity; // 0.0 to 1.0
};

class Music {
public:
    Music();
    const std::vector<Note>& get_notes() const;
    void add_note(int pitch, float duration, float velocity);

private:
    std::vector<Note> notes_;
    int bpm_; // beats per minute
};

} // namespace synth
