#define _USE_MATH_DEFINES  // NOLINT
#include <cmath>
#include <numeric>
#include <span>

#include "ImpulseAudio.hpp"

#include <SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL_error.h>

#include "AudioBuffer.hpp"
#include "Logger.hpp"

static constexpr auto SAMPLE_RATE = 48000;
static constexpr auto BUFFER_SIZE = 1024;

namespace JE
{

    namespace
    {
        std::array<float, SAMPLE_RATE> g_BufferSineLookup;
        AudioBufferView<float> g_SineLookup({g_BufferSineLookup.data(), g_BufferSineLookup.size()});
    }  // namespace

    void Callback(void*, [[maybe_unused]] Uint8* stream_u8, int len)
    {
        auto stream = reinterpret_cast<int32_t*>(stream_u8);  // NOLINT

        len /= 8;  // 8 because /4 for each channel
        EngineLogger()->trace(len);
        g_SineLookup.WriteTo(std::span<int32_t>(stream, static_cast<std::size_t>(len)));
    }

    // cppcheck-suppress unusedFunction
    void ImpulseAudio::TestStuff()
    {
        // Generate single period sine wave into lookup
        auto lut_pos = 0.0f;
        for (auto& lut : g_BufferSineLookup) {
            lut = sinf(static_cast<float>(M_PI) * 2.0f / SAMPLE_RATE * lut_pos++);
        }

        EngineLogger()->trace("[ImpulseAudio Test Stuff]");

        auto audio_initialized = SDL_Init(SDL_INIT_AUDIO) == 0;
        if (!audio_initialized) {
            EngineLogger()->error("SDL audio subsystem initialization failed: {}", SDL_GetError());
            SDL_AudioQuit();
            return;
        }

        auto device_count = SDL_GetNumAudioDevices(0);
        if (device_count != 0) {
            EngineLogger()->info("Found {} audio devices:", device_count);
            for (auto i = 0; i < device_count; i++) {
                SDL_AudioSpec local_spec;
                auto spec_status = SDL_GetAudioDeviceSpec(i, 0, &local_spec);
                const auto* device_name = SDL_GetAudioDeviceName(i, 0);
                if (spec_status == 0) {
                    EngineLogger()->info(" [{}] - {} [{} buffer | {} Hz | {} bit {} | {} ch]",
                                         i,
                                         device_name,
                                         local_spec.samples,
                                         local_spec.freq,
                                         local_spec.format & 0xFF,
                                         local_spec.format & 0x100 ? "Float" : "Integer",  // NOLINT
                                         local_spec.channels);
                } else {
                    EngineLogger()->info(" [{}] - {} [specs not available]", i, device_name);
                }
            }
        } else {
            EngineLogger()->info("No audio devices found :(");
        }

        SDL_AudioSpec specs = {};
        specs.freq = SAMPLE_RATE;
        specs.format = AUDIO_S32;
        specs.channels = 2;
        specs.samples = BUFFER_SIZE;
        specs.callback = Callback;
        SDL_AudioSpec got_spec;
        auto dev = SDL_OpenAudioDevice(
            "Scarlett Solo (3rd Gen.) Analog Stereo", 0, &specs, &got_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
        if (dev == 0) {
            EngineLogger()->warn("Failed to open audio device: {}", SDL_GetError());
            SDL_AudioQuit();
            return;
        }
        EngineLogger()->info("Audio device connected [{} buffer | {} Hz | {} bit {} | {} ch]",
                             got_spec.samples,
                             got_spec.freq,
                             got_spec.format & 0xFF,
                             got_spec.format & 0x100 ? "Float" : "Integer",  // NOLINT
                             got_spec.channels);

        SDL_PauseAudioDevice(dev, 0);
    }

}  // namespace JE