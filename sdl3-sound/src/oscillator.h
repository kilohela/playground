#pragma once

#include <vector>

namespace synth {

enum class Waveform {
    SINE,
    SQUARE,
    SAWTOOTH,
    TRIANGLE
};

class Oscillator {
public:
    Oscillator();
    void set_waveform(Waveform waveform);
    void set_frequency(double freq);
    void set_amplitude(double amp);
    double process();

private:
    Waveform waveform_;
    double frequency_;
    double amplitude_;
    double phase_;
};

} // namespace synth
