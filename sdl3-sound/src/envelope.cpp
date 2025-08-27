#include "envelope.h"
#include "config.h"
#include <fmt/core.h>

namespace synth {

Envelope::Envelope()
    : state_(State::IDLE),
      attack_rate_(0.0),
      decay_rate_(0.0),
      sustain_level_(1.0),
      release_rate_(0.0),
      level_(0.0) {}

// attack: time in seconds for the envelope to reach its peak
void Envelope::set_attack(float attack) {
    attack_rate_ = (attack > 0) ? 1.0 / (attack * kSampleRate) : 1.0;
}

// decay: time in seconds for the envelope to fall to the sustain level
void Envelope::set_decay(float decay) {
    decay_rate_ = (decay > 0) ? 1.0 / (decay * kSampleRate) : 1.0;
}

// sustain: level at which the envelope holds after decay
void Envelope::set_sustain(float sustain) {
    sustain_level_ = sustain;
}

// release: time in seconds for the envelope to fall to zero after note off
void Envelope::set_release(float release) {
    release_rate_ = (release > 0) ? 1.0 / (release * kSampleRate) : 1.0;
}

// generate one sample of the envelope
float Envelope::process() {
    switch (state_) {
        case State::IDLE:
            break;
        case State::ATTACK:
            level_ += attack_rate_;
            if (level_ >= 1.0) {
                level_ = 1.0;
                state_ = State::DECAY;
            }
            break;
        case State::DECAY:
            level_ -= decay_rate_;
            if (level_ <= sustain_level_) {
                level_ = sustain_level_;
                state_ = State::SUSTAIN;
            }
            break;
        case State::SUSTAIN:
            // level_ remains at sustain_level_
            break;
        case State::RELEASE:
            level_ -= release_rate_;
            if (level_ <= 0.0) {
                level_ = 0.0;
                state_ = State::IDLE;
            }
            break;
    }
    return level_;
}

void Envelope::note_on() {
    state_ = State::ATTACK;
    level_ = 0.0;
}

void Envelope::note_off() {
    state_ = State::RELEASE;
}

bool Envelope::is_active() const {
    return state_ != State::IDLE;
}

bool Envelope::is_releasing() const {
    return state_ == State::RELEASE;
}

} // namespace synth
