#include "EpicGameEngine/Application.h"
#ifdef EGE_PLATFORM_WINDOWS
#include "EpicGameEngine/Window/Window.h"
#include "EpicGameEngine/Window/WindowsWindow.h"
#endif
#include <spdlog/spdlog.h>
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/Events/EventConversion.h>
#include <EpicGameEngine/Renderer/Renderer.h>
#include <EpicGameEngine/ImGui/ImGuiLayer.h>

namespace EpicGameEngine
{
	Application::Application()
	= default;
	Application::~Application()
	= default;

	// TODO: Rewrite this to use our new event system when it is complete.
	void Application::Run()
	{
		window = std::unique_ptr<Window>(Window::CreateWindow());

		spdlog::info("EpicGameEngine Initialized");

		SDL_Event event{};
		layers.PushLayer(new ImGuiLayer());
		while (window->running)
		{
			Application::PollEvents(sdlEvent);
			window->OnUpdate();
			GPU_ClearRGBA(Renderer::GetTarget(), 0, 0, 0, 255);
			for (auto l : layers.layers)
			{
				l->OnUpdate();
				l->OnRender();
			}
			window->OnRender();
		}
		Renderer::Shutdown();
	}

	void Application::PollEvents(SDL_Event e)
	{
		if (SDL_PollEvent(&e))
		{
			event = SDL_Event_to_Event(&e);
			Application::OnEvent(event);
			/*for (auto it = layers.layers.rbegin(); it != layers.layers.rend(); it++)
			{
				if (event->handled)
				{
					break;
				}
				(*it)->OnEvent(event);
			}*/
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
