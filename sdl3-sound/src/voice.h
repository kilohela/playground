
#include "oscillator.h"
#include "envelope.h"
#include "config.h"

namespace synth {

class Voice {
public:
    bool is_active(){
        return envelope_.is_active();
    }

    float process();
    void note_on(Waveform waveform, int midi_note, float velocity, float duration);

private:
    float midi_to_frequency(int midi_note);

    Waveform waveform_;
    Oscillator oscillator_;
    Envelope envelope_;
    float time_;
    float duration_;

    // note_off function: envelope_.note_off()
};

} // namespace synth
