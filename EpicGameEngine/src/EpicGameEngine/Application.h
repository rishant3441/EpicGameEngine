#pragma once

#include "Core.h"
namespace EpicGameEngine
{
	class EPICGAMEENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

