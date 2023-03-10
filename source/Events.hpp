#pragma once

#include <string_view>
#include <type_traits>

namespace JE
{

class IEvent
{
  public:
    friend class EventDispatcher;

    IEvent(const IEvent& other) = delete;
    IEvent(IEvent&& other) = delete;
    auto operator=(const IEvent& other) -> IEvent& = delete;
    auto operator=(IEvent&& other) -> IEvent& = delete;

    enum class EventCategory
    {
        UNKNOWN,
        APP
    };

    enum class EventType
    {
        UNKNOWN,
        QUIT
    };

    IEvent() = default;
    virtual ~IEvent() = default;

    virtual auto Category() const -> EventCategory = 0;
    virtual auto Type() const -> EventType = 0;

    inline auto Handled() const -> bool { return m_Handled; }

  private:
    inline void SetEventHandled() { m_Handled = true; }

    bool m_Handled = false;
};

constexpr auto EventCategoryToString(IEvent::EventCategory category) -> std::string_view
{
    if (category == IEvent::EventCategory::APP) {
        return "APP";
    }

    return "UNKNOWN";
}

constexpr auto EventTypeToString(IEvent::EventType type) -> std::string_view
{
    if (type == IEvent::EventType::QUIT) {
        return "QUIT";
    }

    return "UNKNOWN";
}

class IEventProcessor
{
  public:
    IEventProcessor(const IEventProcessor& other) = delete;
    IEventProcessor(IEventProcessor&& other) = delete;
    auto operator=(const IEventProcessor& other) -> IEventProcessor& = delete;
    auto operator=(IEventProcessor&& other) -> IEventProcessor& = delete;

    IEventProcessor() = default;
    virtual ~IEventProcessor() = default;

    virtual void ProcessEvent(IEvent& event) = 0;
};

class EventDispatcher
{
  public:
    explicit EventDispatcher(IEvent& event)
        : m_Event(event)
    {
    }

    template<typename EventType, typename Func>
    inline auto Dispatch(Func func) -> bool
    {
        static_assert(std::is_base_of_v<IEvent, EventType>, "Events have to derive from IEvent");

        if (m_Event.Handled() || EventType::StaticType() != m_Event.Type()) {
            return false;
        }

        if (func(static_cast<EventType&>(m_Event))) {
            m_Event.SetEventHandled();
        }

        return true;
    }

  private:
    IEvent& m_Event;
};

class UnknownEvent final : public IEvent
{
  public:
    using IEvent::IEvent;

    inline auto Category() const -> EventCategory override { return EventCategory::UNKNOWN; }
    inline auto Type() const -> EventType override { return EventType::UNKNOWN; }

    static constexpr auto StaticType() -> EventType { return EventType::UNKNOWN; }
};

class QuitEvent final : public IEvent
{
  public:
    using IEvent::IEvent;

    inline auto Category() const -> EventCategory override { return EventCategory::APP; }
    inline auto Type() const -> EventType override { return EventType::QUIT; }

    static constexpr auto StaticType() -> EventType { return EventType::QUIT; }
};

}  // namespace JE
