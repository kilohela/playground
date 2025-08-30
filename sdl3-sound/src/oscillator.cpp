#include "oscillator.h"
#include "config.h"
#include <cmath>
#include <fmt/core.h>
#include <cassert>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace synth {

void Oscillator::reset(Waveform waveform, float frequency, float amplitude){
    assert(frequency > 0 || amplitude > 0);
    waveform_ = waveform;
    frequency_ = frequency;
    amplitude_ = amplitude;
    phase_ = 0.0;
}


float Oscillator::process() {
    float value = 0.0;
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
        default:
            // should not reach here
            assert(0); 
            value = 0.0;
            break;
    }

    phase_ += frequency_ / kSampleRate;
    if (phase_ >= 1.0) {
        phase_ -= 1.0;
    }

    return value * amplitude_;
}

} // namespace synth
