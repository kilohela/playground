#pragma once

namespace synth {

class Envelope {
public:
    Envelope();
    void set_attack(double attack);
    void set_decay(double decay);
    void set_sustain(double sustain);
    void set_release(double release);
    double process();
    void note_on();
    void note_off();
    bool is_active() const;

private:
    enum class State {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

    State state_;
    double attack_rate_;
    double decay_rate_;
    double sustain_level_;
    double release_rate_;
    double level_;
};

} // namespace synth
