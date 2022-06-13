#include "EpicGameEngine/Application.h"
#ifdef EGE_PLATFORM_WINDOWS
#include "EpicGameEngine/Window/Window.h"
#endif

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
		SDL_Event event{};
		while (window->running)
		{
			window->PollEvents(event);
			window->OnUpdate();
			window->OnRender();
		}
	}
}
