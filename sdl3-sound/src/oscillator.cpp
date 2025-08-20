#include "oscillator.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace synth {

Oscillator::Oscillator()
    : waveform_(Waveform::TRIANGLE),
      frequency_(440.0),
      amplitude_(1.0),
      phase_(0.0) {}

void Oscillator::set_waveform(Waveform waveform) {
    waveform_ = waveform;
}

void Oscillator::set_frequency(double freq) {
    frequency_ = freq;
}

void Oscillator::set_amplitude(double amp) {
    amplitude_ = amp;
}

double Oscillator::process() {
    double value = 0.0;
    switch (waveform_) {
        case Waveform::SINE:
            value = sin(phase_ * 2.0 * M_PI);
            break;
        case Waveform::SQUARE:
            value = (sin(phase_ * 2.0 * M_PI) >= 0) ? 1.0 : -1.0;
            break;
        case Waveform::SAWTOOTH:
            value = 2.0 * (phase_ - floor(phase_ + 0.5));
            break;
        case Waveform::TRIANGLE:
            value = 2.0 * fabs(2.0 * (phase_ - floor(phase_ + 0.5))) - 1.0;
            break;
    }

    phase_ += frequency_ / 44100.0; // Assuming 44100 sample rate
    if (phase_ >= 1.0) {
        phase_ -= 1.0;
    }

    return value * amplitude_;
}

} // namespace synth
