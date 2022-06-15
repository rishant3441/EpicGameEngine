#include "EpicGameEngine/Application.h"
#ifdef EGE_PLATFORM_WINDOWS
#include "EpicGameEngine/Window/Window.h"
#include "EpicGameEngine/Window/WindowsWindow.h"
#endif
#include <spdlog/spdlog.h>
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/Events/EventConversion.h>

namespace EpicGameEngine
{
	Application::Application()
	{
		window = std::unique_ptr<Window>(Window::CreateWindow());
	}
	Application::~Application()
	{

	}

	// TODO: Rewrite this to use our new event system when it is complete.
	void Application::Run()
	{
		spdlog::info("EpicGameEngine Initialized");

		SDL_Event event{};
		while (window->running)
		{
			Application::PollEvents(sdlEvent);
			window->OnUpdate();
			window->OnRender();
		}
	}

	void Application::PollEvents(SDL_Event e)
	{
		if (SDL_PollEvent(&e))
		{
			event = SDL_Event_to_Event(&e);
			Application::OnEvent(event);
		}	
	}

	void Application::OnEvent(std::shared_ptr<Event> e)
	{
		// TODO: Make this more readable
		if (e == nullptr)
			return;
		if (e->GetEventType() == EventType::WindowClose || e->GetEventType() == EventType::WindowResize)
		{
			window->OnEvent(e);
		}
	}
}
