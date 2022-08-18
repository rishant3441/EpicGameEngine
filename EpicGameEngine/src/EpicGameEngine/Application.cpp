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
#include <EpicGameEngine/Input/Input.h>
#include <EpicGameEngine/Renderer/Camera/CameraController.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <SDL_gpu.h>
#include <EpicGameEngine/Debug.h>
#include <EpicGameEngine/Scripting/ScriptingEngine.h>
#include <omp.h>

namespace EpicGameEngine
{
	Application* Application::Instance = nullptr;

	Application::Application()
	{
		Instance = this;
	}
	Application::~Application()
	{
		delete m_ImGuiLayer;
	}

	void Application::Run(int argc, char** argv)
	{
	    ArgCount = argc;
	    Args = *argv;

		window = std::shared_ptr<Window>(Window::CreateWindow());

		m_ImGuiLayer = new ImGuiLayer();
		m_ImGuiLayer->OnAttach();

		CameraController::UpdateCamera();

		Debug::Log::LogInfo("EpicGameEngine Initialized");

		SDL_Event event{};

		window->OnRender();

        Renderer::target->matrix_mode = GPU_PROJECTION;

        Debug::Log::Init();

        for (auto l : layers.layers)
        {
            l->DefferedOnAttach();
        }

        while (window->running)
		{
			float time = (float) SDL_GetTicks() / 1000;
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			Application::PollEvents(sdlEvent);
			window->OnUpdate();
			GPU_ClearRGBA(Renderer::GetTarget(), 0, 0, 0, 255);
			if (Renderer::enableDrawingToTexture)
			{
				GPU_ClearRGBA(Renderer::window, 0, 0, 0, 255);
			}
			for (auto l : layers.layers)
			{
				l->OnUpdate(timestep);
			}
			GPU_FlushBlitBuffer();
			m_ImGuiLayer->BeginFrame();
			for (auto l : layers.layers)
			{
				l->OnImGuiRender();
			}
			m_ImGuiLayer->OnImGuiRender();
			m_ImGuiLayer->EndFrame();
			window->OnRender();
		}
		Renderer::Shutdown();
	}

	void Application::PollEvents(SDL_Event e)
	{
		if (SDL_PollEvent(&e))
		{

			event = SDL_Event_to_Event(&e);
			ImGui_ImplSDL2_ProcessEvent(&e);
			for (auto it = layers.layers.rbegin(); it != layers.layers.rend(); it++)
			{
				if (event == nullptr)
					break;
				if (event->handled)
				{
					break;
				}
				(*it)->OnEvent(event);
			}
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
		if (e->GetEventType() == EventType::MousePressed || e->GetEventType() == EventType::MouseReleased)
		{
			InputSystem.OnEvent(e);
		}
	}

	void Application::Close()
	{
		window->running = false;
		Renderer::Shutdown();
	}
}
