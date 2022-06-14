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
		/*SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow(data.Title.c_str(), 500, 500, data.width, data.height, SDL_WINDOW_RESIZABLE);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // TODO: Potentially move this to a new renderer class in the future.*/ 

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
