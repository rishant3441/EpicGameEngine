#pragma once

#include <EpicGameEngine/ege_pch.h>

/// Has the Core Event Handler for the engine.
/**
 *
 * Defines abstract base Event Class and Event Dispatcher.
 * EGE_BIND_EVENT_FN used to bind an event to the Dispatch function within the dispatcher.
 *
 */

#define EGE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace EpicGameEngine
{
	// TODO: Create derived Event classes based on these EventTypes.

	/*
		Mouse events should store what mouse button was pressed
		Keyboard events should store what key was pressed
	*/
	/// \brief Core Event Types
	enum class EventType
	{
		None = 0,
		MousePressed, MouseReleased, KeyPressed, KeyReleased, // Input Events
		WindowClose, WindowResize // Window Events
	};

	/// \brief Base Event Type which holds basic data about an event.
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		[[nodiscard]] virtual auto GetEventType() const -> EventType = 0;
		[[nodiscard]] virtual auto toString() const -> std::string = 0;

		bool handled = false;
	};
    /// \brief NoEvent class for when no event has occured.
	class NoEvent : public Event
	{
	public:
		NoEvent() = default;
		~NoEvent() override = default;

		[[nodiscard]] auto GetEventType() const -> EventType override { return EventType::None; }
		[[nodiscard]] auto toString() const -> std::string override 
		{
			return "No Event";
		} 
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }

	/// \brief Use to "dispatch" events to their respective function handlers.
	class EventDispatcher
	{
	public:
		EventDispatcher(std::shared_ptr<Event> event)
			: event(event)
		{
		}

		/**
		 * Dispatches events. Used in tandem with EGE_BIND_EVENT_FN
		 * @tparam T Binds an event to a function callback
		 * @param func The function that is called when the binded event is received.
		 */
		template<typename T, typename F>
		auto Dispatch(const F& func) -> bool
		{
			if (event->GetEventType() == T::GetStaticType())
			{
				event->handled |= func(static_cast<T&>(*event));
				return true;
			}
			return false;
		}
	private:
		std::shared_ptr<Event> event;
	};
	
}