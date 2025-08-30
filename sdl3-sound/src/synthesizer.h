#pragma once

#include "oscillator.h"
#include "envelope.h"
#include "music.h"
#include "config.h"
#include "voice.h"
#include <cstdint>
#include <forward_list>

namespace synth {

class Synthesizer {
public:
    Synthesizer();
    void process(float* buffer, int num_frames);
    void play_music(Music& music);

private:

    Voice voice_pool_[kMaxVoices];
    std::forward_list<Voice*> free_voice_;

    Music* current_music_;

    // time in ticks (1/sample rate) since the start of the music
    uint32_t tick_;
};

} // namespace synth
