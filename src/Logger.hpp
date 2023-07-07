#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
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
            enum class SpdlogSinks
            {
                CONSOLE_SINK,
                FILE_SINK,
                COUNT
            };

            enum class SpdlogLoggers
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
                    m_LogSinks[EnumToSizeT(SpdlogSinks::CONSOLE_SINK)] =
                        CreateRef<spdlog::sinks::ansicolor_stdout_sink_mt>();
                } else if constexpr (JE::PLATFORM_WINDOWS) {
                    m_LogSinks[EnumToSizeT(SpdlogSinks::CONSOLE_SINK)] =
                        CreateRef<spdlog::sinks::stdout_color_sink_mt>();
                }

                const auto NOW = std::chrono::system_clock::now();
                const auto IN_TIME_T = std::chrono::system_clock::to_time_t(NOW);

                std::stringstream datetime;
                datetime << std::put_time(std::localtime(&IN_TIME_T),  // NOLINT(concurrency-mt-unsafe)
                                          "%Y-%m-%d_%H-%M-%S");

                m_LogSinks[EnumToSizeT(SpdlogSinks::FILE_SINK)] =
                    CreateRef<spdlog::sinks::basic_file_sink_mt>("JEngine3D_" + datetime.str() + ".log", true);

                m_LogSinks[EnumToSizeT(SpdlogSinks::CONSOLE_SINK)]->set_pattern("%^[%T] [%l] %n: %v%$");
                m_LogSinks[EnumToSizeT(SpdlogSinks::FILE_SINK)]->set_pattern("[%T] [%l] %n: %v");

                m_SpdlogLoggers[EnumToSizeT(SpdlogLoggers::JENGINE_LOGGER)] =
                    CreateRef<spdlog::logger>("JEngine", std::begin(m_LogSinks), std::end(m_LogSinks));
                m_SpdlogLoggers[EnumToSizeT(SpdlogLoggers::APP_LOGGER)] =
                    CreateRef<spdlog::logger>("App", std::begin(m_LogSinks), std::end(m_LogSinks));

                for (const auto& logger : m_SpdlogLoggers) {
                    logger->set_level(spdlog::level::trace);
                    // logger->flush_on(spdlog::level::trace);
                }
            }

            std::array<Ref<spdlog::sinks::sink>, EnumToSizeT(SpdlogSinks::COUNT)> m_LogSinks;
            std::array<Ref<spdlog::logger>, EnumToSizeT(SpdlogLoggers::COUNT)> m_SpdlogLoggers;
        };

        inline const JEngineLoggers LOGGERS;  // NOLINT(cert-err58-cpp)

    }  // namespace detail

    inline auto EngineLogger() -> Ref<spdlog::logger>
    {
        return detail::LOGGERS.m_SpdlogLoggers[EnumToSizeT(detail::JEngineLoggers::SpdlogLoggers::JENGINE_LOGGER)];
    }

    inline auto AppLogger() -> Ref<spdlog::logger>
    {
        return detail::LOGGERS.m_SpdlogLoggers[EnumToSizeT(detail::JEngineLoggers::SpdlogLoggers::APP_LOGGER)];
    }

}  // namespace JE