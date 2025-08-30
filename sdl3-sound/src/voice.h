
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
    struct status {
        Waveform waveform;
        int midi_note;
    } status;
    void note_on(Waveform waveform, int midi_note, float velocity);
    void note_off();

private:
    float midi_to_frequency(int midi_note);

    Oscillator oscillator_;
    Envelope envelope_;

    // note_off function: envelope_.note_off()
};

} // namespace synth
