#define _USE_MATH_DEFINES  // NOLINT(bugprone-reserved-identifier, cert-dcl37-c, cert-dcl51-cpp,
                           // readability-identifier-naming)
#include <cmath>
#include <numeric>
#include <span>

#include "ImpulseAudio.hpp"

#include <SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL_error.h>

#include "Logger.hpp"

#ifndef M_PIf
#    define M_PIf static_cast<float>(M_PI)  // NOLINT(readability-identifier-naming)
#endif

namespace JE
{

unsigned int audioPos;  // NOLINT
float audioVolume = static_cast<float>(0x7FFFFFFF) * 0.1f;  // NOLINT
float audioPhase = 0;  // NOLINT

void Callback(void*, Uint8* stream, int len)  // NOLINT
{
    auto dataView =
        std::span<Sint32>{reinterpret_cast<Sint32*>(stream),  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
                          static_cast<std::size_t>(len / 4)};

    static float sval = 60.0f;  // NOLINT
    float audio_step = (sval / (48000.0f)) * M_PIf;  // NOLINT
    if (sval >= 30)  // NOLINT
        sval -= 10.0f / ((48000.0f) / static_cast<float>(len));  // NOLINT

    for (auto& data : dataView) {
        float w0 = sinf(audioPhase);  // NOLINT

        data = static_cast<Sint32>(audioVolume * w0);
        audioPhase += audio_step;

        audioVolume -= (static_cast<float>(0x7FFFFFFF) * 0.025f) / ((48000.0f));  // NOLINT
        if (audioVolume < 0) {
            audioVolume = 0;
        }
    }
}

// cppcheck-suppress unusedFunction
void ImpulseAudio::TestStuff()
{
    EngineLogger()->trace("[ImpulseAudio Test Stuff]");

    auto audioInitialized = SDL_Init(SDL_INIT_AUDIO) == 0;
    if (!audioInitialized) {
        EngineLogger()->error("SDL audio subsystem initialization failed: {}", SDL_GetError());
        SDL_AudioQuit();
        return;
    }

    auto deviceCount = SDL_GetNumAudioDevices(0);
    if (deviceCount != 0) {
        EngineLogger()->info("Found {} audio devices:", deviceCount);
        for (auto i = 0; i < deviceCount; i++) {
            SDL_AudioSpec localSpec;
            auto specStatus = SDL_GetAudioDeviceSpec(i, 0, &localSpec);
            const auto* deviceName = SDL_GetAudioDeviceName(i, 0);
            if (specStatus == 0) {
                EngineLogger()->info(" [{}] - {} [{} buffer | {} Hz | {} bit {} | {} ch]",
                                     i,
                                     deviceName,
                                     localSpec.samples,
                                     localSpec.freq,
                                     localSpec.format & 0xFF,  // NOLINT
                                     (localSpec.format & 0x100) != 0 ? "Float" : "Integer",  // NOLINT
                                     localSpec.channels);
            } else {
                EngineLogger()->info(" [{}] - {} [specs not available]", i, deviceName);
            }
        }
    } else {
        EngineLogger()->info("No audio devices found :(");
    }

    SDL_AudioSpec specs = {};
    specs.freq = 48000;  // NOLINT
    specs.format = AUDIO_S32;
    specs.channels = 2;
    specs.samples = 1024;  // NOLINT
    specs.callback = Callback;
    SDL_AudioSpec gotSpec;
    auto dev =
        SDL_OpenAudioDevice("Scarlett Solo (3rd Gen.) Analog Stereo", 0, &specs, &gotSpec, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (dev == 0) {
        EngineLogger()->warn("Failed to open audio device: {}", SDL_GetError());
        SDL_AudioQuit();
        return;
    }
    EngineLogger()->info("Audio device connected [{} buffer | {} Hz | {} bit {} | {} ch]",
                         gotSpec.samples,
                         gotSpec.freq,
                         gotSpec.format & 0xFF,  // NOLINT
                         (gotSpec.format & 0x100) != 0 ? "Float" : "Integer",  // NOLINT
                         gotSpec.channels);

    SDL_PauseAudioDevice(dev, 0);
}

}  // namespace JE