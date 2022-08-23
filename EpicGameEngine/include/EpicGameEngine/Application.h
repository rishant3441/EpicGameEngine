#pragma once

#include "Core.h"
#include <EpicGameEngine/Window/Window.h>
#include <EpicGameEngine/Layers/LayerArray.h>
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/Input/Input.h>
#include <EpicGameEngine/ImGui/ImGuiLayer.h>
#include <EpicGameEngine/Timestep.h>
#include <EpicGameEngine/ege_pch.h>
namespace EpicGameEngine
{
	class EPICGAMEENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run(int argc, char** argv);
        void GameLoop();
        void OnEvent(std::shared_ptr<Event> e);
		void Close();

		static Application& Get() { return *Instance;  }

        int ArgCount = 0;
        char* Args = nullptr;

        SDL_Event sdlEvent{};
        std::shared_ptr<Event> event = nullptr;

        // Application Systems
		std::shared_ptr<Window> window;

		LayerArray layers;

		Input InputSystem;

		ImGuiLayer* m_ImGuiLayer;

		// Allocators
		CoreLib::BumpAllocator MainAllocator = CoreLib::BumpAllocator(1024 * 1024);
		CoreLib::BumpAllocator FrameAllocator = CoreLib::BumpAllocator(1024 * 1024);

	private:
		void PollEvents(SDL_Event e);

	private:
		static Application* Instance;
		float lastFrameTime = 0.0f;
	};

	// To be defined in CLIENT
	Application* CreateApplication(int argc, char** argv);
}

