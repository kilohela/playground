/* simple-playback.c ... */

/*
 * This example code creates a simple audio stream for playing sound, and
 * generates a sine wave sound effect for it to play as time goes on. This
 * is the simplest way to get up and running with procedural sound.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_AudioStream *stream = NULL;
static int current_sine_sample = 0;
SDL_AudioSpec spec;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    

    SDL_SetAppMetadata("Example Audio Simple Playback", "1.0", "com.example.audio-simple-playback");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* we don't _need_ a window for audio-only things but it's good policy to have one. */
    if (!SDL_CreateWindowAndRenderer("examples/audio/simple-playback", 640, 480, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* We're just playing a single thing here, so we'll use the simplified option.
       We are always going to feed audio in as mono, float32 data at 8000Hz.
       The stream will convert it to whatever the hardware wants on the other side. */
    spec.channels = 1;
    spec.format = SDL_AUDIO_F32;
    spec.freq = 44100;
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    if (!stream) {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* SDL_OpenAudioDeviceStream starts the device paused. You have to tell it to start! */
    SDL_ResumeAudioStreamDevice(stream);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const char *message = "Hello World!";
    int w = 0, h = 0;
    float x, y;
    const float scale = 4.0f;

    /* Center the message and scale it up */
    SDL_GetRenderOutputSize(renderer, &w, &h);
    SDL_SetRenderScale(renderer, scale, scale);
    x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
    y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

    /* Draw the message */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, x, y, message);
    /* see if we need to feed the audio stream more data yet.
       We're being lazy here, but if there's less than half a second queued, generate more.
       A sine wave is unchanging audio--easy to stream--but for video games, you'll want
       to generate significantly _less_ audio ahead of time! */
    const int minimum_audio = (float)spec.freq / 2 * sizeof (float); // 0.5 seconds of audio at 44100Hz
    if (SDL_GetAudioStreamQueued(stream) < minimum_audio) {
        static float samples[1024];  /* this will feed 512 samples each frame until we get to our maximum. */
        int i;

        /* generate a 440Hz pure tone */
        for (i = 0; i < SDL_arraysize(samples); i++) {
            const int freq = 440;
            int tone = ((float)current_sine_sample/spec.freq)*12;  /* make it a musical tone */
            const float phase = current_sine_sample * freq / (float)spec.freq;
            // samples[i] = 0.3 * (SDL_sinf(phase * 2 * SDL_PI_F) >= 0 ? 1.0f : -1.0f);
            samples[i] = 0.3 * (2.0f / M_PI) * asin(sinf(phase * 2 * SDL_PI_F * pow(2, (float)(tone%12)/12)));  /* triangle wave */
            current_sine_sample++;
        }

        /* wrapping around to avoid floating-point errors */
        current_sine_sample %= spec.freq;

        /* feed the new data to the stream. It will queue at the end, and trickle out as the hardware needs more data. */
        SDL_PutAudioStreamData(stream, samples, sizeof (samples));
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
