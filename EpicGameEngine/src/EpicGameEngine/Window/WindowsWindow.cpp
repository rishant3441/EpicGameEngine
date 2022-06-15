#include "EpicGameEngine/ege_pch.h"
#include "EpicGameEngine/Window/WindowsWindow.h"
#include "EpicGameEngine/Events/Event.h"
#include "EpicGameEngine/Events/EventConversion.h"
#include "EpicGameEngine/Events/WindowEvent.h"
#include "spdlog/spdlog.h"

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
		Init(data);
	}

	WindowsWindow::~WindowsWindow()
	{
	}

	void WindowsWindow::Init(const WindowData& data)
	{	
		window = SDL_CreateWindow(data.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, data.width, data.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		GPU_SetInitWindow(SDL_GetWindowID(window));
		target = GPU_Init(data.width, data.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	}

	void WindowsWindow::OnUpdate()
	{

	}

	void WindowsWindow::OnRender()
	{
		GPU_ClearRGBA(target, 0, 0, 0, 255);
		GPU_Flip(target);
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
