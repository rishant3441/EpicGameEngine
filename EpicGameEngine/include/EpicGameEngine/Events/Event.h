#pragma once

#define EGE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace EpicGameEngine
{
	// TODO: Create derived Event classes based on these EventTypes.

	/*
		Mouse events should store what mouse button was pressed
		Keyboard events should store what key was pressed
	*/
	enum class EventType
	{
		None = 0,
		MousePressed, MouseReleased, KeyPressed, KeyReleased, // Input Events
		WindowClose, WindowResize // Window Events
	};

	class Event
	{
	public:
		Event();
		virtual ~Event();

		virtual EventType GetEventType() const = 0;
		virtual void toString() const = 0;

		bool handled = false;
	};

	// Use to "dispatch" events to their respective function handlers. 
	class EventDispatcher
	{
		EventDispatcher(Event& event)
			: event(event)
		{
		}

		//	This is from TheCherno - I honestly don't know how it works 
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (event.GetEventType() == T::GetEventType())
			{
				event.handled |= func(static_cast<T&>(event));
				return true;
			}
			return false;
		}
	private:
		Event& event;
	};
	
	// TODO: Implement this after derived EventTypes are created.
	/*Event SDL_Event_to_Event(SDL_Event* event)
	{

	}*/
}