#include <math.h>

#define SIZEOF(x) (sizeof(x) / sizeof((x)[0]))


const int BPM = 120;  /* beats per minute */
const int sample_rate = 44100;  /* samples per second */
const int samples_per_beat = (60 * sample_rate) / BPM;

const int audio_buffer_size = 10 * sample_rate;  /* 10 seconds of audio buffer */

float audio_buffer[audio_buffer_size];

int audio_buffer_write_pos = 0;
int audio_buffer_read_pos = 0;


enum wave {
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTH
};

typedef struct {
    enum wave wavetype;
    float frequency;      /* in Hz */
    float amplitude;      /* from 0.0 to 1.0 */
    float duration;       /* in beats */
    float intro_time;     /* in beats */
    float fade_out_time;  /* in beats */
} WaveSpec;

void generate_wave(WaveSpec *spec, int start_sample) {
    int samples_to_generate = spec->duration * samples_per_beat;  /* convert beats to samples */
    int intro_samples = spec->intro_time * samples_per_beat;
    int fade_out_samples = spec->fade_out_time * samples_per_beat;

    for (int i = 0; i < samples_to_generate; i++) {
        float t = (float)i / sample_rate;  /* time in seconds */
        float value = 0.0f;

        switch (spec->wavetype) {
            case SINE:
                value = spec->amplitude * sinf(2.0f * M_PI * spec->frequency * t);
                break;
            case SQUARE:
                value = spec->amplitude * (sinf(2.0f * M_PI * spec->frequency * t) >= 0 ? 1.0f : -1.0f);
                break;
            case TRIANGLE:
                value = spec->amplitude * (2.0f / M_PI) * asin(sinf(2.0f * M_PI * spec->frequency * t));
                break;
            case SAWTOOTH:
                value = spec->amplitude * (2.0f * (t * spec->frequency - floor(t * spec->frequency + 0.5f)));
                break;
        }

        if (i < intro_samples) {
            value *= (float)i / intro_samples;  /* fade in */
        } else if (i >= samples_to_generate - fade_out_samples) {
            value *= (float)(samples_to_generate - i) / fade_out_samples;  /* fade out */
        }

        audio_buffer[start_sample + i] += value;
    }
}