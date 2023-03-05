#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////

#include <string>
#include <string_view>

#include <fmt/core.h>

#define JE_ASSERT_BREAK_ON_FAIL false
#include "Application.hpp"
#include "Assert.hpp"
#include "Base.hpp"
#include "Events.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "Platform.hpp"

namespace JE
{
struct Size2D;
}  // namespace JE

struct TestGraphicsContext : JE::IGraphicsContext
{
    inline auto Created() const -> bool override { return true; }
};

struct TestWindow : JE::IWindow
{
    inline auto Created() const -> bool override { return true; }
    inline auto GraphicsContext() -> JE::IGraphicsContext& override
    {
        return m_GraphicsContext;
    }

    TestGraphicsContext m_GraphicsContext;
};

struct TestPlatform : JE::IPlatform
{
    inline auto Name() const -> std::string_view override
    {
        return "TestPlatform";
    }

    inline auto Initialize() -> bool override { return true; }
    inline auto Initialized() const -> bool override { return true; }
    inline auto GetLastError() const -> std::string_view override { return ""; }

    inline auto PollEvents([[maybe_unused]] JE::IEventProcessor& eventProcessor)
        -> bool override
    {
        return false;
    }

    inline auto CreateWindow([[maybe_unused]] std::string_view title,
                             [[maybe_unused]] const JE::Size2D& size)
        -> JE::IWindow* override
    {
        return m_Windows.emplace_back(JE::CreateScope<TestWindow>()).get();
    }

    JE::Vector<JE::Scope<TestWindow>>
        m_Windows;  // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes,
                    // readability-identifier-naming)
};

TEST_CASE(  // NOLINT(cert-err58-cpp,
            // cppcoreguidelines-avoid-non-const-global-variables)
    "Test Base macros",
    "[Base]")
{
    enum class TestEnum
    {
        ZERO,
        ONE,
        TWO,
        COUNT
    };

    REQUIRE(JE_STRINGIFY_MACRO(test == notest)
            == std::string("test == notest"));

    REQUIRE(JE::EnumToInt(TestEnum::ZERO) == 0);
    REQUIRE(JE::EnumToInt(TestEnum::ONE) == 1);

    REQUIRE(JE::EnumToSizeT(TestEnum::ONE) == 1);
    REQUIRE(JE::EnumToSizeT(TestEnum::TWO) == 2);
}

TEST_CASE("Test Assert", "[Assert]")
{
    struct Library
    {
        std::string m_Name = fmt::format("{}", "JEngine-Reformed");
    };

    const Library LIB{};

    REQUIRE(ASSERT(LIB.m_Name == "JEngine-Reformed") == true);
    REQUIRE(ASSERT(LIB.m_Name == "JEngine-Old") == false);
}

TEST_CASE("Test Loggers", "[Logger]")
{
    REQUIRE(JE::EngineLogger() != nullptr);
    REQUIRE(JE::AppLogger() != nullptr);
}

TEST_CASE(
    "Test Platform initialization, window creation and OpenGL context creation",
    "[Platform][OpenGL]")
{
    REQUIRE(JE::EnginePlatform().Initialize());

    auto* window = JE::CreateWindow("TestWindow");
    REQUIRE(window->Created());
    REQUIRE(window->GraphicsContext().Created());
}

TEST_CASE("Test Application creation and main loop", "[Application]")
{
    REQUIRE(JE::Application().Initialized());
    REQUIRE(JE::Application().LoopCount() == 0);

    JE::Application().Loop(1);

    REQUIRE(JE::Application().LoopCount() == 1);
    REQUIRE(JE::Application().EventsProcessed() != 0);
}

TEST_CASE("Test StaticType and Category/Type to string", "[Events]")
{
    REQUIRE(JE::UnknownEvent::StaticType() == JE::IEvent::EventType::UNKNOWN);
    REQUIRE(JE::QuitEvent::StaticType() == JE::IEvent::EventType::QUIT);

    REQUIRE(JE::EventCategoryToString(JE::IEvent::EventCategory::UNKNOWN)
            == "UNKNOWN");
    REQUIRE(JE::EventCategoryToString(JE::IEvent::EventCategory::APP) == "APP");

    REQUIRE(JE::EventTypeToString(JE::IEvent::EventType::UNKNOWN) == "UNKNOWN");
    REQUIRE(JE::EventTypeToString(JE::IEvent::EventType::QUIT) == "QUIT");
}

TEST_CASE("Test EventDispatcher and Event handling", "[Events]")
{
    JE::UnknownEvent event;

    JE::EventDispatcher dispatcher{event};
    REQUIRE(!event.Handled());

    bool dispatched = dispatcher.Dispatch<JE::UnknownEvent>(
        []([[maybe_unused]] auto& evnt) { return true; });

    REQUIRE(dispatched);
    REQUIRE(event.Handled());

    dispatched = dispatcher.Dispatch<JE::UnknownEvent>(
        []([[maybe_unused]] auto& evnt) { return true; });

    REQUIRE(!dispatched);
}

TEST_CASE("Test Application initialization failure (Initialization failure)",
          "[Application]")
{
    struct InitFailPlatform : TestPlatform
    {
        inline auto Initialize() -> bool override { return false; }
    };

    JE::detail::InjectCustomEnginePlatform<InitFailPlatform>();

    REQUIRE(!JE::Application().Initialized());
}

TEST_CASE("Test Application initialization failure (Window creation failure)",
          "[Application]")
{
    struct FailWindow : TestWindow
    {
        inline auto Created() const -> bool override { return false; }
    };

    struct WindowFailPlatform : TestPlatform
    {
        inline auto CreateWindow([[maybe_unused]] std::string_view title,
                                 [[maybe_unused]] const JE::Size2D& size)
            -> JE::IWindow* override
        {
            return m_Windows.emplace_back(JE::CreateScope<FailWindow>()).get();
        }
    };

    JE::detail::InjectCustomEnginePlatform<WindowFailPlatform>();

    REQUIRE(!JE::Application().Initialized());
}

TEST_CASE("Test Application QuitEvent handling", "[Application][Events]")
{
    struct QuitEventPlatform : TestPlatform
    {
        inline auto PollEvents(JE::IEventProcessor& eventProcessor)
            -> bool override
        {
            if (!m_EventProcessed) {
                JE::QuitEvent event;
                eventProcessor.ProcessEvent(event);

                m_EventProcessed = true;
                return true;
            }

            return false;
        }

        bool m_EventProcessed = false;
    };

    JE::detail::InjectCustomEnginePlatform<QuitEventPlatform>();

    JE::Application().Loop();

    REQUIRE(!JE::Application().Running());
}

// TEST_CASE("Test ", "[Application][Events]")
// {
//     struct QuitEventPlatform : TestPlatform
//     {
//         inline auto PollEvents(JE::IEventProcessor& eventProcessor)
//             -> bool override
//         {
//             if (!m_EventProcessed) {
//                 JE::QuitEvent event;
//                 eventProcessor.ProcessEvent(event);

//                 m_EventProcessed = true;
//                 return true;
//             }

//             return false;
//         }

//         bool m_EventProcessed = false;
//     };

//     JE::detail::InjectCustomEnginePlatform<QuitEventPlatform>();

//     JE::Application().Loop();

//     REQUIRE(!JE::Application().Running());
// }