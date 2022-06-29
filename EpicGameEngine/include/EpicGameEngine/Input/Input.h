#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/Events/InputEvent.h>
#include <EpicGameEngine/Input/MouseCodes.h>
#include <EpicGameEngine/Input/KeyCodes.h>

namespace EpicGameEngine
{
	class EPICGAMEENGINE_API Input
	{
	public:
		void OnEvent(std::shared_ptr<Event> e);

		static bool isMouseButtonPressed(MouseCode button);
		static float GetMouseX();
		static float GetMouseY();
	private:
		bool OnMouseReleased(MouseReleasedEvent& e);
		bool OnMousePressed(MousePressedEvent& e);
	};
}