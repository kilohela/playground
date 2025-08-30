#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <fmt/core.h>
#include "synthesizer.h"
#include "config.h"

static synth::Synthesizer* synthesizer = nullptr;
static synth::Music music(180);
static SDL_AudioStream *stream = nullptr;
static SDL_AudioSpec spec;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {

#ifdef __SANITIZE_ADDRESS__
    // AddressSanitizer is enabled
    #warning "ASan is enabled"
#endif
    SDL_SetAppMetadata("Synthesizer Prototype", "1.0", "com.example.synthesizer");

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    spec.freq = kSampleRate;
    spec.format = SDL_AUDIO_F32;
    spec.channels = 1;

    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    if (!stream) {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create a simple melody
    music.note_on(0.0, synth::Waveform::TRIANGLE, 60, 0.8);
    music.note_off(0.5, synth::Waveform::TRIANGLE, 60);
    music.note_on(0.5, synth::Waveform::TRIANGLE, 67, 0.8);
    music.note_off(1.0, synth::Waveform::TRIANGLE, 67);
    music.note_on(1.0, synth::Waveform::TRIANGLE, 64, 0.8);
    music.note_off(1.5, synth::Waveform::TRIANGLE, 64);
    music.note_on(1.5, synth::Waveform::TRIANGLE, 67, 0.8);
    music.note_off(2.0, synth::Waveform::TRIANGLE, 67);
    music.note_on(2.0, synth::Waveform::TRIANGLE, 72, 0.8);
    music.note_off(2.5, synth::Waveform::TRIANGLE, 72);
    music.note_on(2.5, synth::Waveform::TRIANGLE, 67, 0.8);
    music.note_off(3.0, synth::Waveform::TRIANGLE, 67);
    music.note_on(3.0, synth::Waveform::TRIANGLE, 64, 0.8);
    music.note_off(3.5, synth::Waveform::TRIANGLE, 64);
    music.note_on(3.5, synth::Waveform::TRIANGLE, 67, 0.8);
    music.note_off(4.0, synth::Waveform::TRIANGLE, 67);
    music.note_on(4.0, synth::Waveform::TRIANGLE, 74, 0.8);
    music.note_off(4.5, synth::Waveform::TRIANGLE, 74);
    music.note_on(4.5, synth::Waveform::TRIANGLE, 67, 0.8);
    music.note_off(5.0, synth::Waveform::TRIANGLE, 67);
    music.note_on(5.0, synth::Waveform::TRIANGLE, 64, 0.8);
    music.note_off(5.5, synth::Waveform::TRIANGLE, 64);
    music.note_on(5.5, synth::Waveform::TRIANGLE, 67, 0.8);
    music.note_off(6.0, synth::Waveform::TRIANGLE, 67);
    music.note_on(6.0, synth::Waveform::TRIANGLE, 76, 0.8);
    music.note_off(6.5, synth::Waveform::TRIANGLE, 76);
    music.note_on(6.5, synth::Waveform::TRIANGLE, 67, 0.8);
    music.note_off(7.0, synth::Waveform::TRIANGLE, 67);
    music.note_on(7.0, synth::Waveform::TRIANGLE, 77, 0.8);
    music.note_off(7.5, synth::Waveform::TRIANGLE, 77);
    music.note_on(7.5, synth::Waveform::TRIANGLE, 67, 0.8);
    music.note_off(8.0, synth::Waveform::TRIANGLE, 67);
    music.note_on(8.0, synth::Waveform::TRIANGLE, 79, 0.8);
    music.note_off(9.0, synth::Waveform::TRIANGLE, 79);

    synthesizer = new synth::Synthesizer();
    synthesizer->play_music(music);

    SDL_ResumeAudioStreamDevice(stream);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        fmt::print("Quit\n");
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    const int minimum_audio = (float)spec.freq / 2 * sizeof (float); // 0.5 seconds
    if (SDL_GetAudioStreamQueued(stream) < minimum_audio) {
        static float samples[1024];
        synthesizer->process(samples, SDL_arraysize(samples));
        SDL_PutAudioStreamData(stream, samples, sizeof(samples));
    }
    SDL_Delay(10); // Avoid busy-waiting
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_CloseAudioDevice(1);
    delete synthesizer;
    SDL_Quit();
}

