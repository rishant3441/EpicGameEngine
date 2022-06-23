#pragma once
#include <utility>

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

		WindowData(std::string title = "Epic Game Engine", unsigned int width = 1280, unsigned int height = 720)
			: Title(std::move(title)), width(width), height(height)
		{}
	};
	class EPICGAMEENGINE_API Window
	{
	public:
		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;

		[[nodiscard]] virtual auto GetWidth() const -> unsigned int = 0;
		[[nodiscard]] virtual auto GetHeight() const -> unsigned int = 0;

		virtual void OnEvent(std::shared_ptr<Event> e) = 0;

		static auto CreateWindow(const WindowData& data = WindowData()) -> Window*;

		SDL_Renderer* renderer; 

		inline static bool running;
	};
}