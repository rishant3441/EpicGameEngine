#pragma once

#include "Core.h"
#include <EpicGameEngine/Window/Window.h>
namespace EpicGameEngine
{
	class EPICGAMEENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

		std::unique_ptr<Window> window;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

