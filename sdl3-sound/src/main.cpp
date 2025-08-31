#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <fmt/core.h>
#include "synthesizer.h"
#include "config.h"

static synth::Synthesizer* synthesizer = nullptr;
static synth::Music music(80);
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

    int lead = music.add_channel(synth::Waveform::TRIANGLE);
    int bass = music.add_channel(synth::Waveform::SAWTOOTH, 0.3);

    music.time_ = 0.0f;

    music.note_on(lead, "C5");

    music.time_ = 0.5f;

    music.note_off(lead, "C5");
    music.note_on(lead, "C5");

    music.time_ = 1.0f;

    music.note_off(lead, "C5");
    music.note_on(lead, "G5");

    music.time_ = 1.5f;

    music.note_off(lead, "G5");
    music.note_on(lead, "G5");

    music.time_ = 2.0f;

    music.note_off(lead, "G5");
    music.note_on(lead, "A5");

    music.time_ = 2.5f;

    music.note_off(lead, "A5");
    music.note_on(lead, "A5");

    music.time_ = 3.0f;

    music.note_off(lead, "A5");
    music.note_on(lead, "G5");

    music.time_ = 4.0f;

    music.note_off(lead, "G5");
    music.note_on(lead, "F5");

    music.time_ = 4.5f;

    music.note_off(lead, "F5");
    music.note_on(lead, "F5");

    music.time_ = 5.0f;

    music.note_off(lead, "F5");
    music.note_on(lead, "E5");

    music.time_ = 5.5f;

    music.note_off(lead, "E5");
    music.note_on(lead, "E5");

    music.time_ = 6.0f;

    music.note_off(lead, "E5");
    music.note_on(lead, "D5");

    music.time_ = 6.5f;

    music.note_off(lead, "D5");
    music.note_on(lead, "D5");

    music.time_ = 7.0f;

    music.note_off(lead, "D5");
    music.note_on(lead, "C5");

    music.time_ = 8.0f;

    music.note_off(lead, "C5");


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

