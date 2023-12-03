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
#undef CreateWindowA
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
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
        window = SDL_CreateWindow(data.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, data.width, data.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        SDL_GL_CreateContext(window);
		Renderer::Init();
	}

	void WindowsWindow::OnUpdate()
	{
	}

	void WindowsWindow::OnRender()
	{
        SDL_GL_SwapWindow(window);
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
        Renderer::SetViewport(0, 0, e.GetNewWidth(), e.GetNewHeight());
		return true;
	}
}
