#pragma once

#include "Assert.hpp"
#include "Events.hpp"
#include "Platform.hpp"

namespace JE
{

class App final : public IEventProcessor
{
  public:
    static constexpr auto MAINWINDOW_DEFAULT_TITLE = "JEngine-Reformed Application";

    App()
    {
        if (!EnginePlatform().Initialize()) {
            EngineLogger()->error("Failed to create application - EnginePlatform failed to initialize");
            return;
        }

        m_MainWindow = CreateWindow(MAINWINDOW_DEFAULT_TITLE);
        if (!m_MainWindow->Created()) {
            EngineLogger()->error("Failed to create application - MainWindow could not be created");
            return;
        }

        m_Initialized = true;
    }

    // cppcheck-suppress unusedFunction
    inline void ProcessEvent(IEvent& event) override
    {
        EngineLogger()->trace("Processing event of class - {} | type - {}",
                              EventCategoryToString(event.Category()),
                              EventTypeToString(event.Type()));

        EventDispatcher dispatcher{event};
        dispatcher.Dispatch<QuitEvent>(
            [this]([[maybe_unused]] const QuitEvent& evnt)
            {
                m_Running = false;
                return true;
            });
    }

    inline void ProcessEvents()
    {
        ASSERT(m_Initialized);

        while (EnginePlatform().PollEvents(*this)) {
            ++m_EventsProcessed;
        }
    }

    inline void Loop(std::int64_t loopCount = -1)
    {
        ASSERT(m_Initialized);

        m_Running = true;
        while (m_LoopCount != loopCount && m_Running) {
            ProcessEvents();

            ++m_LoopCount;
        }
    }

    inline auto LoopCount() const { return m_LoopCount; }
    inline auto Running() const { return m_Running; }
    inline auto EventsProcessed() const { return m_EventsProcessed; }

    inline auto Initialized() const { return m_Initialized; }

  private:
    IWindow* m_MainWindow = nullptr;

    std::int64_t m_LoopCount = 0;
    bool m_Running = false;
    std::uint64_t m_EventsProcessed = 0;

    bool m_Initialized = false;
};

inline auto Application() -> App&
{
    static App sApplication;
    return sApplication;
}

}  // namespace JE