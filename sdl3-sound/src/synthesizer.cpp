#include "synthesizer.h"
#include <cmath>

namespace synth {

Synthesizer::Synthesizer(int sample_rate)
    : sample_rate_(sample_rate),
      current_music_(nullptr),
      current_note_index_(0),
      time_(0.0) {
    envelope_.set_attack(0.01);
    envelope_.set_decay(0.1);
    envelope_.set_sustain(0.8);
    envelope_.set_release(0.2);
}

void Synthesizer::process(float* buffer, int num_frames) {
    for (int i = 0; i < num_frames; ++i) {
        if (current_music_ && current_note_index_ < current_music_->get_notes().size()) {
            const auto& note = current_music_->get_notes()[current_note_index_];
            if (time_ >= note.duration) {
                current_note_index_++;
                time_ = 0.0;
                if (current_note_index_ < current_music_->get_notes().size()) {
                    const auto& next_note = current_music_->get_notes()[current_note_index_];
                    oscillator_.set_frequency(midi_to_frequency(next_note.pitch));
                    oscillator_.set_amplitude(next_note.velocity);
                    envelope_.note_on();
                } else {
                    envelope_.note_off();
                }
            }
        }

        double osc_sample = oscillator_.process();
        double env_sample = envelope_.process();
        buffer[i] = static_cast<float>(osc_sample * env_sample);

        time_ += 1.0 / sample_rate_;
    }
}

void Synthesizer::play_music(const Music& music) {
    current_music_ = &music;
    current_note_index_ = 0;
    time_ = 0.0;

    if (!music.get_notes().empty()) {
        const auto& first_note = music.get_notes()[0];
        oscillator_.set_frequency(midi_to_frequency(first_note.pitch));
        oscillator_.set_amplitude(first_note.velocity);
        envelope_.note_on();
    }
}

double Synthesizer::midi_to_frequency(int midi_note) {
    return 440.0 * pow(2.0, (midi_note - 69.0) / 12.0);
}

} // namespace synth
