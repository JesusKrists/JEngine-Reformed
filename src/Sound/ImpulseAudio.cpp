#include <numeric>

#include "ImpulseAudio.hpp"

#include <SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL_error.h>

#include "Logger.hpp"

#ifndef M_PIf
#    define M_PIf static_cast<float>(M_PI)
#endif

namespace JE
{

unsigned int audio_pos;
float audio_volume = static_cast<float>(0x7FFFFFFF) * 0.1f;
float audio_phase = 0;

void callback(void*, Uint8* stream, int len)
{
    len /= 4;

    static float sval = 60.0f;
    float audio_step = (sval / (48000.0f)) * M_PIf;
    if (sval >= 30)
        sval -= 10.0f / ((48000.0f) / static_cast<float>(len));

    int i;
    // cppcheck-suppress cstyleCast
    Sint32* buf = reinterpret_cast<Sint32*>(stream);
    for (i = 0; i < len; i++) {
        float w0 = sinf(audio_phase);

        buf[i] = static_cast<Sint32>(audio_volume * w0);
        audio_phase += audio_step;

        audio_volume -= (static_cast<float>(0x7FFFFFFF) * 0.025f) / ((48000.0f));
        if (audio_volume < 0)
            audio_volume = 0;
    }
}

// cppcheck-suppress unusedFunction
void ImpulseAudio::TestStuff()
{
    EngineLogger()->trace("[ImpulseAudio Test Stuff]");

    if (SDL_Init(SDL_INIT_AUDIO)) {
        EngineLogger()->error("SDL audio subsystem initialization failed: {}", SDL_GetError());
        SDL_AudioQuit();
        return;
    }

    auto device_count = SDL_GetNumAudioDevices(0);
    if (device_count) {
        EngineLogger()->info("Found {} audio devices:", device_count);
        for (auto i = 0; i < device_count; i++) {
            SDL_AudioSpec local_spec;
            auto spec_status = SDL_GetAudioDeviceSpec(i, 0, &local_spec);
            auto dev_name = SDL_GetAudioDeviceName(i, 0);
            if (spec_status) {
                EngineLogger()->info(" [{}] - {} [specs not available]", i, dev_name);
            } else {
                EngineLogger()->info(" [{}] - {} [{} buffer | {} Hz | {} bit {} | {} ch]",
                                     i,
                                     dev_name,
                                     local_spec.samples,
                                     local_spec.freq,
                                     local_spec.format & 0xFF,
                                     local_spec.format & 0x100 ? "Float" : "Integer",
                                     local_spec.channels);
            }
        }
    } else {
        EngineLogger()->info("No audio devices found :(");
    }

    SDL_AudioSpec specs = {};
    specs.freq = 48000;
    specs.format = AUDIO_S32;
    specs.channels = 2;
    specs.samples = 1024;
    specs.callback = callback;
    SDL_AudioSpec got_spec;
    auto dev =
        SDL_OpenAudioDevice("Scarlett Solo (3rd Gen.) Analog Stereo", 0, &specs, &got_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (!dev) {
        EngineLogger()->error("Failed to open audio device: {}", SDL_GetError());
        SDL_AudioQuit();
        return;
    }
    EngineLogger()->info("Audio device connected [{} buffer | {} Hz | {} bit {} | {} ch]",
                         got_spec.samples,
                         got_spec.freq,
                         got_spec.format & 0xFF,
                         got_spec.format & 0x100 ? "Float" : "Integer",
                         got_spec.channels);

    SDL_PauseAudioDevice(dev, 0);
}

}  // namespace JE