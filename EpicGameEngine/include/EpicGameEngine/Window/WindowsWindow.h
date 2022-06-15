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

		virtual ~WindowsWindow();

		void OnUpdate() override;
		void OnRender() override;

		void OnEvent(std::shared_ptr<Event> e) override;

		bool OnWindowCloseEvent(WindowCloseEvent& e);

		bool OnWindowResizeEvent(WindowResizeEvent& e);

		inline unsigned int GetWidth() const override { return data.width; }
		inline unsigned int GetHeight() const override { return data.height; }
	private:
		void Init(const WindowData& data);
		void Shutdown();

	private:
		WindowData data;
	public:
		SDL_Window* window;
		GPU_Target* target;
	};
}