#include "EpicGameEngine/ege_pch.h"
#include "EpicGameEngine/Window/WindowsWindow.h"
#include <SDL.h>

namespace EpicGameEngine
{
	Window* WindowsWindow::CreateWindow(const WindowData& data)
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
		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow(data.Title.c_str(), 500, 500, data.width, data.height, SDL_RENDERER_ACCELERATED);
		printf("Could not create window");
	}
	void WindowsWindow::OnUpdate()
	{

	}
}
