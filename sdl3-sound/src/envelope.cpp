#include "envelope.h"

namespace synth {

Envelope::Envelope()
    : state_(State::IDLE),
      attack_rate_(0.0),
      decay_rate_(0.0),
      sustain_level_(1.0),
      release_rate_(0.0),
      level_(0.0) {}

void Envelope::set_attack(double attack) {
    attack_rate_ = (attack > 0) ? 1.0 / (attack * 44100.0) : 1.0;
}

void Envelope::set_decay(double decay) {
    decay_rate_ = (decay > 0) ? 1.0 / (decay * 44100.0) : 1.0;
}

void Envelope::set_sustain(double sustain) {
    sustain_level_ = sustain;
}

void Envelope::set_release(double release) {
    release_rate_ = (release > 0) ? 1.0 / (release * 44100.0) : 1.0;
}

double Envelope::process() {
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

} // namespace synth
