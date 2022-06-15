#pragma once
#include "EpicGameEngine/Core.h"
#include "EpicGameEngine/ege_pch.h"
#include "EpicGameEngine/Events/Event.h"

namespace EpicGameEngine
{
	struct EPICGAMEENGINE_API WindowData
	{
	public:
		std::string Title;
		unsigned int width;
		unsigned int height;

		WindowData(const std::string& title = "Epic Game Engine", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), width(width), height(height)
		{}
	};
	class EPICGAMEENGINE_API Window
	{
	public:
		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void OnEvent(std::shared_ptr<Event> e) = 0;

		static Window* CreateWindow(const WindowData& data = WindowData());

		SDL_Renderer* renderer; 

		inline static bool running;
	};
}