#pragma once

#include <chrono>
#include <iomanip>

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4365)
#endif

#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

#include "Base.hpp"

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
            m_logSinks[EnumToSizeT(SpdlogSinks::CONSOLE_SINK)] =
                std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
        } else if constexpr (JE::PLATFORM_WINDOWS) {
            m_logSinks[EnumToSizeT(SpdlogSinks::CONSOLE_SINK)] =
                std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        }

        const auto NOW = std::chrono::system_clock::now();
        const auto IN_TIME_T = std::chrono::system_clock::to_time_t(NOW);

        std::stringstream datetime;
        datetime << std::put_time(
            std::localtime(&IN_TIME_T),  // NOLINT(concurrency-mt-unsafe)
            "%Y-%m-%d %X");

        m_logSinks[EnumToSizeT(SpdlogSinks::FILE_SINK)] =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                "JEngine3D_" + datetime.str() + ".log", true);

        m_logSinks[EnumToSizeT(SpdlogSinks::CONSOLE_SINK)]->set_pattern(
            "%^[%T] [%l] %n: %v%$");
        m_logSinks[EnumToSizeT(SpdlogSinks::FILE_SINK)]->set_pattern(
            "[%T] [%l] %n: %v");

        m_spdlogLoggers[EnumToSizeT(SpdlogLoggers::JENGINE_LOGGER)] =
            std::make_shared<spdlog::logger>(
                "JEngine", std::begin(m_logSinks), std::end(m_logSinks));
        m_spdlogLoggers[EnumToSizeT(SpdlogLoggers::APP_LOGGER)] =
            std::make_shared<spdlog::logger>(
                "App", std::begin(m_logSinks), std::end(m_logSinks));

        for (auto& sink : m_logSinks) {
            if (sink.get() == nullptr) {
                DEBUGBREAK();
            }
        }

        for (auto& logger : m_spdlogLoggers) {
            if (logger.get() == nullptr) {
                DEBUGBREAK();
            }

            logger->set_level(spdlog::level::trace);
            // logger->flush_on(spdlog::level::trace);
        }
    }

    std::array<spdlog::sink_ptr, EnumToSizeT(SpdlogSinks::COUNT)> m_logSinks;
    std::array<std::shared_ptr<spdlog::logger>,
               EnumToSizeT(SpdlogLoggers::COUNT)>
        m_spdlogLoggers;
};

inline const JEngineLoggers
    LOGGERS;  // NOLINT(cert-err58-cpp,
              // cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace detail

inline auto EngineLogger() -> std::shared_ptr<spdlog::logger>
{
    return detail::LOGGERS.m_spdlogLoggers[EnumToSizeT(
        detail::JEngineLoggers::SpdlogLoggers::JENGINE_LOGGER)];
}

inline auto AppLogger() -> std::shared_ptr<spdlog::logger>
{
    return detail::LOGGERS.m_spdlogLoggers[EnumToSizeT(
        detail::JEngineLoggers::SpdlogLoggers::APP_LOGGER)];
}

}  // namespace JE