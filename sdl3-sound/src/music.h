#pragma once

#include <vector>

namespace synth {

struct Note {
    int pitch; // MIDI note number
    double duration; // in seconds
    double velocity; // 0.0 to 1.0
};

class Music {
public:
    Music();
    const std::vector<Note>& get_notes() const;
    void add_note(int pitch, double duration, double velocity);

private:
    std::vector<Note> notes_;
};

} // namespace synth
