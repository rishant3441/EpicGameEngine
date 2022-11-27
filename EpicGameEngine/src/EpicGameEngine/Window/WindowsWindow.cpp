#include "EpicGameEngine/ege_pch.h"
#include "EpicGameEngine/Window/WindowsWindow.h"
#include "EpicGameEngine/Events/Event.h"
#include "EpicGameEngine/Events/EventConversion.h"
#include "EpicGameEngine/Events/WindowEvent.h"
#include "spdlog/spdlog.h"
#include "EpicGameEngine/Renderer/Renderer.h"
#include "EpicGameEngine/Application.h"

namespace EpicGameEngine
{
	Window* Window::CreateWindow(const WindowData& data)
	{
	    return new WindowsWindow(data);
	}

	WindowsWindow::WindowsWindow(const WindowData& data)
		: data(data)
	{
		running = true;
		WindowsWindow::Init(data);
	}

	WindowsWindow::~WindowsWindow()
	= default;

	void WindowsWindow::Init(const WindowData& data)
	{	
		window = SDL_CreateWindow(data.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, data.width, data.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		Renderer::Init(data);
	}

	void WindowsWindow::OnUpdate()
	{
		if (Renderer::enableDrawingToTexture)
		{
		}
	}

	void WindowsWindow::OnRender()
	{
		Renderer::Render();
	}

    void WindowsWindow::OnEvent(std::shared_ptr<Event> e)
	{
		EventDispatcher eventDispatcher(e);
		
		eventDispatcher.Dispatch<WindowCloseEvent>(EGE_BIND_EVENT_FN(WindowsWindow::OnWindowCloseEvent));
		eventDispatcher.Dispatch<WindowResizeEvent>(EGE_BIND_EVENT_FN(WindowsWindow::OnWindowResizeEvent));
	}

	bool WindowsWindow::OnWindowCloseEvent(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}
	
	bool WindowsWindow::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		GPU_SetWindowResolution(e.GetNewWidth(), e.GetNewHeight());
		return true;
	}
}
