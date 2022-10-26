#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/Events/InputEvent.h>
#include <EpicGameEngine/Input/MouseCodes.h>
#include <EpicGameEngine/Input/KeyCodes.h>

/**
 *  Interface to check for mouse and keyboard inputs
 */

namespace EpicGameEngine
{
	class EPICGAMEENGINE_API Input
	{
	public:
		void OnEvent(std::shared_ptr<Event> e);

		/**
		 * Checks if a mouse button is pressed.
		 * @param button Mouse Code that should be checked.
		 * @return Returns true if pressed.
		 */
		static bool isMouseButtonPressed(MouseCode button);
        /**
         * Checks if a key is pressed.
         * @param key Key Code that should be checked.
         * @return Returns true if pressed.
         */
		static bool isKeyPressed(KeyCode key);
		/// \brief Returns Mouse X Position.
		static float GetMouseX();
		/// \brief Returns Mouse Y Position.
		static float GetMouseY();

	private:
		bool OnMouseReleased(MouseReleasedEvent& e);
		bool OnMousePressed(MousePressedEvent& e);
	};
}
