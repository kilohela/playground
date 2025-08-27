#include "synthesizer.h"
#include "config.h"
#include <cassert>
#include <cmath>

namespace synth {

Synthesizer::Synthesizer()
    : current_music_(nullptr),
      current_note_index_(0),
      time_(0.0) {

    for (int i = 0; i < kMaxVoices; ++i) {
        free_voice_.push_front(&voice_pool_[i]);
    }
}

// load the audio stream into buffer, num_frames is the number of sample points
void Synthesizer::process(float* buffer, int num_frames) {
    for (int i = 0; i < num_frames; ++i) {
        if (current_music_ && current_note_index_ < current_music_->get_notes().size()) {
            const auto& note = current_music_->get_notes()[current_note_index_];
            if (time_ >= note.duration) {
                current_note_index_++;
                time_ = 0.0;
                if (current_note_index_ < current_music_->get_notes().size()) {
                    const auto& next_note = current_music_->get_notes()[current_note_index_];
                    assert(!free_voice_.empty());
                    Voice* new_voice_ptr = free_voice_.front();
                    free_voice_.pop_front();
                    new_voice_ptr->note_on(Waveform::TRIANGLE, next_note.pitch, next_note.velocity, next_note.duration);
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

        time_ += 1.0 / kSampleRate;
    }
}

// initialize Synthesizer with a Music object
// should be called before process()
void Synthesizer::play_music(const Music& music) {
    current_music_ = &music;
    current_note_index_ = 0;
    time_ = 0.0;

    // load the first note
    if (!music.get_notes().empty()) {
        const auto& note = music.get_notes()[0];
        assert(!free_voice_.empty());
        Voice* new_voice_ptr = free_voice_.front();
        free_voice_.pop_front();
        new_voice_ptr->note_on(Waveform::TRIANGLE, note.pitch, note.velocity, note.duration);
    }
}

} // namespace synth
