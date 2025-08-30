#pragma once

#include <vector>

namespace synth {

enum class Waveform {
    NONE,
    SINE,
    SQUARE,
    SAWTOOTH,
    TRIANGLE
};

class Oscillator {
public:
    float process();
    void reset(Waveform waveform, float frequency, float amplitude);

private:
    Waveform waveform_;
    float frequency_;
    float amplitude_;
    float phase_;
};

} // namespace synth
