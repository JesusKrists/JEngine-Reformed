#pragma once

#include <chrono>
#include <iomanip>
#include <string>

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4365)
#endif

// IWYU pragma: begin_exports
#include <spdlog/logger.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
// IWYU pragma: end_exports

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

#include "Base.hpp"
#include "Memory.hpp"

namespace JE
{

namespace detail  // NOLINT(readability-identifier-naming)
{
struct JEngineLoggers
{
    enum struct SpdlogSinks
    {
        CONSOLE_SINK,
        FILE_SINK,
        COUNT
    };

    enum struct SpdlogLoggers
    {
        JENGINE_LOGGER,
        APP_LOGGER,
        COUNT
    };

    JEngineLoggers()
    {
        spdlog::set_level(spdlog::level::trace);
        spdlog::set_pattern("[%T] [%l] %n: %v");

        if constexpr (JE::PLATFORM_UNIX) {
            mLogSinks[EnumToSizeT(SpdlogSinks::CONSOLE_SINK)] =
                CreateRef<spdlog::sinks::ansicolor_stdout_sink_mt>();
        } else if constexpr (JE::PLATFORM_WINDOWS) {
            mLogSinks[EnumToSizeT(SpdlogSinks::CONSOLE_SINK)] =
                CreateRef<spdlog::sinks::stdout_color_sink_mt>();
        }

        const auto NOW = std::chrono::system_clock::now();
        const auto IN_TIME_T = std::chrono::system_clock::to_time_t(NOW);

        std::stringstream datetime;
        datetime << std::put_time(
            std::localtime(&IN_TIME_T),  // NOLINT(concurrency-mt-unsafe)
            "%Y-%m-%d_%H-%M-%S");

        mLogSinks[EnumToSizeT(SpdlogSinks::FILE_SINK)] =
            CreateRef<spdlog::sinks::basic_file_sink_mt>(
                "JEngine3D_" + datetime.str() + ".log", true);

        mLogSinks[EnumToSizeT(SpdlogSinks::CONSOLE_SINK)]->set_pattern(
            "%^[%T] [%l] %n: %v%$");
        mLogSinks[EnumToSizeT(SpdlogSinks::FILE_SINK)]->set_pattern(
            "[%T] [%l] %n: %v");

        mSpdlogLoggers[EnumToSizeT(SpdlogLoggers::JENGINE_LOGGER)] =
            CreateRef<spdlog::logger>(
                "JEngine", std::begin(mLogSinks), std::end(mLogSinks));
        mSpdlogLoggers[EnumToSizeT(SpdlogLoggers::APP_LOGGER)] =
            CreateRef<spdlog::logger>(
                "App", std::begin(mLogSinks), std::end(mLogSinks));

        for (const auto& sink : mLogSinks) {
            if (sink == nullptr) {
                DEBUGBREAK();
            }
        }

        for (const auto& logger : mSpdlogLoggers) {
            if (logger == nullptr) {
                DEBUGBREAK();
            }

            logger->set_level(spdlog::level::trace);
            // logger->flush_on(spdlog::level::trace);
        }
    }

    std::array<Ref<spdlog::sinks::sink>, EnumToSizeT(SpdlogSinks::COUNT)>
        mLogSinks;
    std::array<Ref<spdlog::logger>, EnumToSizeT(SpdlogLoggers::COUNT)>
        mSpdlogLoggers;
};

inline const JEngineLoggers LOGGERS;  // NOLINT(cert-err58-cpp)

}  // namespace detail

inline auto EngineLogger() -> Ref<spdlog::logger>
{
    return detail::LOGGERS.mSpdlogLoggers[EnumToSizeT(
        detail::JEngineLoggers::SpdlogLoggers::JENGINE_LOGGER)];
}

inline auto AppLogger() -> Ref<spdlog::logger>
{
    return detail::LOGGERS.mSpdlogLoggers[EnumToSizeT(
        detail::JEngineLoggers::SpdlogLoggers::APP_LOGGER)];
}

}  // namespace JE