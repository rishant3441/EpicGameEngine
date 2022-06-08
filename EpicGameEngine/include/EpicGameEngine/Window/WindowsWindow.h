#pragma once

#include "EpicGameEngine/Core.h"
#include "EpicGameEngine/Window/Window.h"
#include <SDL.h>

namespace EpicGameEngine
{
	class EPICGAMEENGINE_API WindowsWindow : public Window
	{
	public: 
		WindowsWindow(const WindowData& data = WindowData());

		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return data.width; }
		inline unsigned int GetHeight() const override { return data.height; }

		Window* CreateWindow(const WindowData& data = WindowData());
	private:
		void Init(const WindowData& data);
		void Shutdown();

	private:
		WindowData data;
	public:
		SDL_Window* window;
	};
}