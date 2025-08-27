#include "oscillator.h"
#include "envelope.h"
#include "voice.h"
#include "config.h"
#include <cassert>
#include <math.h>

namespace synth {

float Voice::process() {
    float osc_sample = oscillator_.process();
    float env_sample = envelope_.process();
    time_ += 1.0 / kSampleRate;
    if (time_ >= duration_ && envelope_.is_active() && envelope_.is_releasing() == false) {
        envelope_.note_off();
    }
    
    return static_cast<float>(osc_sample * env_sample);
}

void Voice::note_on(Waveform waveform, int midi_note, float velocity, float duration) {
    oscillator_.reset(waveform, midi_to_frequency(midi_note), velocity);
    envelope_.note_on();
    time_ = 0.0;
    duration_ = duration;
}

float Voice::midi_to_frequency(int midi_note){
    assert(midi_note >= 0 && midi_note <= 127);
    return 440.0 * pow(2.0, (midi_note - 69.0) / 12.0);
}



} // namespace synth