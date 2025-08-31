#include "synthesizer.h"
#include "config.h"
#include <cassert>
#include <cmath>
#include <cstdint>

namespace synth {

Synthesizer::Synthesizer()
    : current_music_(nullptr),
      tick_(0.0) {

    for (int i = 0; i < kMaxVoices; ++i) {
        free_voice_.push_front(&voice_pool_[i]);
    }
}

// load the audio stream into buffer, num_frames is the number of sample points
void Synthesizer::process(float* buffer, int num_frames) {
    for (int i = 0; i < num_frames; ++i) {
        if(current_music_->has_notes()){
            const auto& note = current_music_->get_note();
            if (tick_ >= note.tick) {
                current_music_->pop_note();
                if(note.on){
                    assert(!free_voice_.empty());
                    Voice* new_voice_ptr = free_voice_.front();
                    free_voice_.pop_front();
                    new_voice_ptr->note_on(note.timbre, note.pitch, note.velocity);
                } else {
                    // note off
                    for(int j = 0; j < kMaxVoices; ++j){
                        Voice& voice = voice_pool_[j];
                        if(voice.is_active() && voice.status.waveform == note.timbre && voice.status.midi_note == note.pitch){
                            voice.note_off();
                            break;
                        }
                    }
                }
            }
        }

        float sample = 0.0f;

        for (int j = 0; j < kMaxVoices; ++j) {
            Voice& voice = voice_pool_[j];
            if(voice.is_active() == false) continue;
            sample += voice.process();
            if (!(voice.is_active())) {
                free_voice_.push_front(&voice);
            }
        }

        buffer[i] = fmin(sample, 1.0f);

        tick_ += 1;
    }
}

// initialize Synthesizer with a Music object
// should be called before process()
void Synthesizer::play_music(Music& music) {
    current_music_ = &music;
    tick_ = 0.0;
}

} // namespace synth
