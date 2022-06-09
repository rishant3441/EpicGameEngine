#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Input/MouseCodes.h>

namespace EpicGameEngine
{
	class Input
	{
	public:
		static bool isMouseButtonPressed(MouseCode button);
		static float GetMouseX();
		static float GetMouseY();
	};
}