#include <EpicGameEngine/Input/Input.h>
#include <EpicGameEngine/Events/Event.h>

namespace EpicGameEngine
{
	void Input::OnEvent(std::shared_ptr<Event> e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MousePressedEvent>(EGE_BIND_EVENT_FN(Input::OnMousePressed));
		dispatcher.Dispatch<MouseReleasedEvent>(EGE_BIND_EVENT_FN(Input::OnMouseReleased));
	}

	// TODO: Rewrite this to use our Event system instead of unreadable SDL code
	bool Input::isMouseButtonPressed(const MouseCode button)
	{
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		if (button == 0 && (buttons & SDL_BUTTON_LMASK) != 0)
		{
			return true;
		}

		if (button == 1 && (buttons & SDL_BUTTON_RMASK) != 0)
		{
			return true;
		}

		if (button == 2 && (buttons & SDL_BUTTON_MMASK) != 0)
		{
			return true;
		}
		return false;
	}

	float Input::GetMouseX()
	{
		int x;
		Uint32 buttons = SDL_GetMouseState(&x, NULL);
		return (float) x;
	}

	float Input::GetMouseY()
	{
		int y;
		Uint32 buttons = SDL_GetMouseState(NULL, &y);
		return (float) y;
	}

	bool Input::isKeyPressed(KeyCode key)
	{
		// TODO: Make this better
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (key == Keyboard::W)
			if (state[SDL_SCANCODE_W])
				return true;
			else
				return false;
		if (key == Keyboard::A)
			if (state[SDL_SCANCODE_A])
				return true;
			else
				return false;

		if (key == Keyboard::S)
			if (state[SDL_SCANCODE_S])
				return true;
			else
				return false;

		if (key == Keyboard::D)
			if (state[SDL_SCANCODE_D])
				return true;
			else
				return false;
		
		if (key == Keyboard::E)
			if (state[SDL_SCANCODE_E])
				return true;
			else
				return false;

		if (key == Keyboard::R)
			if (state[SDL_SCANCODE_R])
				return true;
			else
				return false;
        if (key == Keyboard::LeftAlt)
            if (state[SDL_SCANCODE_LALT])
                return true;
            else
                return false;
		return false;
	}

	bool Input::OnMouseReleased(MouseReleasedEvent& e)
	{
		return true;
	}
	bool Input::OnMousePressed(MousePressedEvent& e)
	{
		return true;
	}
	
}