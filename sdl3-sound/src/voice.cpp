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
    
    return static_cast<float>(osc_sample * env_sample);
}

void Voice::note_on(Waveform waveform, int midi_note, float velocity) {
    status.waveform = waveform;
    status.midi_note = midi_note;
    oscillator_.reset(waveform, midi_to_frequency(midi_note), velocity);
    envelope_.note_on();
}

void Voice::note_off() {
    status.waveform = Waveform::NONE;
    envelope_.note_off();
}

float Voice::midi_to_frequency(int midi_note){
    assert(midi_note >= 0 && midi_note <= 127);
    return 440.0 * pow(2.0, (midi_note - 69.0) / 12.0);
}



} // namespace synth