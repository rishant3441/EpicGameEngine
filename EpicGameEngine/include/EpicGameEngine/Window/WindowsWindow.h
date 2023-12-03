#pragma once

#include "EpicGameEngine/Core.h"
#include "EpicGameEngine/Window/Window.h"
#include "EpicGameEngine/ege_pch.h"
#include "EpicGameEngine/Events/WindowEvent.h"

namespace EpicGameEngine
{
	class EPICGAMEENGINE_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowData& data = WindowData());
		~WindowsWindow() override;

		void OnUpdate() override;
		void OnRender() override;

        // Window Events
		void OnEvent(std::shared_ptr<Event> e) override;

		auto OnWindowCloseEvent(WindowCloseEvent& e) -> bool;
		auto OnWindowResizeEvent(WindowResizeEvent& e) -> bool;

        // Getters
		[[nodiscard]] inline auto GetWidth() const -> unsigned int override { return data.width; }
		[[nodiscard]] inline auto GetHeight() const -> unsigned int override { return data.height; }

	private:
		void Init(const WindowData& data);
		void Shutdown();

	private:
		WindowData data;

	public:
        // Renderer Targets
		inline static SDL_Window* window;
	};
}
