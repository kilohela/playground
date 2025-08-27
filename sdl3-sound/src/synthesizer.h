#pragma once

#include "oscillator.h"
#include "envelope.h"
#include "music.h"
#include "config.h"
#include "voice.h"
#include <forward_list>

namespace synth {

class Synthesizer {
public:
    Synthesizer();
    void process(float* buffer, int num_frames);
    void play_music(const Music& music);

private:

    Voice voice_pool_[kMaxVoices];
    std::forward_list<Voice*> free_voice_;

    const Music* current_music_;
    int current_note_index_;

    // time in seconds since the start of the current note
    float time_;
};

} // namespace synth
