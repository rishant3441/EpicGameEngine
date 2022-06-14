#include "EpicGameEngine/ege_pch.h"
#include "EpicGameEngine/Window/WindowsWindow.h"

namespace EpicGameEngine
{
	Window* Window::CreateWindow(const WindowData& data)
	{
		return new WindowsWindow(data);
	}

	WindowsWindow::WindowsWindow(const WindowData& data)
		: data(data)
	{
		Init(data);
	}

	WindowsWindow::~WindowsWindow()
	{
	}

	void WindowsWindow::Init(const WindowData& data)
	{	
		window = GPU_Init(data.width, data.height, GPU_DEFAULT_INIT_FLAGS);
	}

	void WindowsWindow::OnUpdate()
	{

	}

	void WindowsWindow::OnRender()
	{
		GPU_ClearRGBA(window, 0, 0, 0, 255);
		GPU_Flip(window);
	}
	
	// TODO: Rewrite this to use our new Event system when completed.
	void WindowsWindow::PollEvents(SDL_Event event)
	{
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				GPU_Quit();
				running = false; 
				break;
			}
		}
	}
}
