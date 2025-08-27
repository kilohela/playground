#pragma once

namespace synth {

class Envelope {
public:
    Envelope();
    void set_attack(float attack);
    void set_decay(float decay);
    void set_sustain(float sustain);
    void set_release(float release);
    float process();
    void note_on();
    void note_off();
    bool is_active() const;
    bool is_releasing() const;

private:
    enum class State {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

    State state_;
    float attack_rate_;
    float decay_rate_;
    float sustain_level_;
    float release_rate_;
    float level_;
};

} // namespace synth
