#include <EpicGameEngine/Input/Input.h>

namespace EpicGameEngine
{
	// TODO: Rewrite this to use our Event system instead of unreadable SDL code
	auto Input::isMouseButtonPressed(const MouseCode button) -> bool
	{
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		if (button == 0 && (buttons & SDL_BUTTON_LMASK) != 0)
		{
			return true;
		}
		else
			return false;

		if (button == 1 && (buttons & SDL_BUTTON_RMASK) != 0)
		{
			return true;
		}
		else
			return false;
		
		if (button == 2 && (buttons & SDL_BUTTON_MMASK) != 0)
		{
			return true;
		}
		else
			return false;
	}

	auto Input::GetMouseX() -> float
	{
		int x;
		Uint32 buttons = SDL_GetMouseState(&x, NULL);
		return (float) x;
	}

	auto Input::GetMouseY() -> float
	{
		int y;
		Uint32 buttons = SDL_GetMouseState(NULL, &y);
		return (float) y;
	}
}