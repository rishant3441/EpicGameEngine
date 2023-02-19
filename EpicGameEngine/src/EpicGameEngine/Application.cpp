// TODO: Clean up all these headers
#include <EpicGameEngine/Application.h>
#ifdef EGE_PLATFORM_WINDOWS
#include "EpicGameEngine/Window/Window.h"
#include "EpicGameEngine/Window/WindowsWindow.h"
#endif
#ifdef EGE_PLATFORM_LINUX
#include "EpicGameEngine/Window/Window.h"
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
#include <EpicGameEngine/ege_pch.h>
#include <omp.h>

#include <ranges>
#include <EpicGameEngine/CoreLib/CoreLib.h>

namespace EpicGameEngine
{
    // Singleton
	Application* Application::Instance = nullptr;

	Application::Application()
	{
		Instance = this;
	}
	Application::~Application()
	{
	}

	void Application::GameLoop()
	{
	        // Calculate Delta Time
            float time = (float) SDL_GetTicks() / 1000;
            timestep = time - lastFrameTime;
            lastFrameTime = time;

            // Loops through layers and calls their OnUpdate
            std::for_each(std::execution::par, layers.layers.begin(), layers.layers.end(), [&](auto l){
                l->OnUpdate(timestep);
            });

            //lightingSystem->Render();
	}

	void Application::RenderLoop()
    {
        // Needed for ImGui
        GPU_FlushBlitBuffer();

        // ImGui Rendering
        m_ImGuiLayer->BeginFrame();

        std::for_each(std::execution::par, layers.layers.begin(), layers.layers.end(), [&](auto l){
            l->OnImGuiRender();
        });

        m_ImGuiLayer->OnImGuiRender();
        m_ImGuiLayer->EndFrame();

        // Final full render to screen (Swapping Buffers)
        window->OnRender();
    }

    void Application::Shutdown()
    {
	    Debug::Log::LogInfo("Epic Game Engine Shutting Down...");
        MainAllocator.clear();
        FrameAllocator.clear();
        Renderer::Shutdown();
    }

    // Application Startup
    void Application::Run(int argc, char** argv)
	{
	    // Stores arguments passed through
	    ArgCount = argc;
	    Args = *argv;

	    // Creates a window
        window = std::shared_ptr<Window>(Window::CreateWindow());

		// Creates the seperate ImGui Layer
		m_ImGuiLayer = MainAllocator.allocate<ImGuiLayer>();
		m_ImGuiLayer->OnAttach();

		// Initializes Lighting System
		lightingSystem = MainAllocator.allocate<Lighting>();
		lightingSystem->Init();

		// First render to initialize stuff
		window->OnRender();

		// Inits Logger
        Debug::Log::Init();

        Debug::Log::LogInfo("EpicGameEngine Initialized");

        // OnAttach function that runs after the Engine is initialized
        // For example, loading textures doesn't work without a renderer.
        for (auto l : layers.layers)
        {
            l->DefferedOnAttach();
        }

        while (window->running)
		{
            // Reset Frame Allocator
            FrameAllocator.clear();

            // Runs Event Loop
			Application::PollEvents(sdlEvent);

			// Set Background Color. TODO: Set a way to change this in the editor perhaps?
			GPU_ClearRGBA(Renderer::GetTarget(), 0, 0, 0, 255);
			if (Renderer::enableDrawingToTexture)
			{
				GPU_ClearRGBA(Renderer::window, 0, 0, 0, 255);
			}

			// Game Loop - Updating objects, layers, etc.
            GameLoop();

			// Render Loop - Actually renders objects and widgets to the screen.
			RenderLoop();
        }

        // Shutdown Process
        Shutdown();
	}

	void Application::PollEvents(SDL_Event e)
	{
		if (SDL_PollEvent(&e))
		{
		    // Converts an SDL_Event to our own managed Event System
			event = SDL_Event_to_Event(&e);

			// ImGui has their own way of handling SDL_Events.
			ImGui_ImplSDL2_ProcessEvent(&e);

			// Go through all the layers and pass events to them.
			for (auto& layer : std::ranges::reverse_view(layers.layers))
			{
				if (event == nullptr)
					break;
				if (event->handled)
				{
					break;
				}
				layer->OnEvent(event);
			}

			// Calls the Application's OnEvent
			Application::OnEvent(event);
		}
	}

	void Application::OnEvent(std::shared_ptr<Event> e)
	{
	    // Goes through all of the system OnEvents and calls them
		// TODO: Make this more readable

		// Returns if the event doesn't exist
		if (e == nullptr)
			return;

		// Window Events
		if (e->GetEventType() == EventType::WindowClose || e->GetEventType() == EventType::WindowResize)
		{
			window->OnEvent(e);
		}

		// Mouse Events
		if (e->GetEventType() == EventType::MousePressed || e->GetEventType() == EventType::MouseReleased)
		{
			InputSystem.OnEvent(e);
		}
	}

	void Application::Close()
	{
		window->running = false;
	}
}
