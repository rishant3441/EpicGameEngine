#pragma once

#include "Core.h"
#include <EpicGameEngine/Window/Window.h>
#include <EpicGameEngine/Layers/LayerArray.h>
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/Input/Input.h>
#include <EpicGameEngine/ege_pch.h>
namespace EpicGameEngine
{
	class EPICGAMEENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

		void OnEvent(std::shared_ptr<Event> e);

		std::unique_ptr<Window> window;

		SDL_Event sdlEvent{};
		std::shared_ptr<Event> event = nullptr; 

		LayerArray layers; 

		Input InputSystem;
	private: 
		void PollEvents(SDL_Event e);	
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

