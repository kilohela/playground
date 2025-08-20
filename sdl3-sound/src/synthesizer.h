#pragma once

#include "oscillator.h"
#include "envelope.h"
#include "music.h"
#include <vector>

namespace synth {

class Synthesizer {
public:
    Synthesizer(int sample_rate);
    void process(float* buffer, int num_frames);
    void play_music(const Music& music);

private:
    double midi_to_frequency(int midi_note);

    int sample_rate_;
    Oscillator oscillator_;
    Envelope envelope_;

    const Music* current_music_;
    int current_note_index_;
    double time_;
};

} // namespace synth
