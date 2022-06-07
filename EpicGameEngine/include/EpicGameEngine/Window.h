#pragma once

namespace EpicGameEngine
{
	class EPICGAMEENGINE_API Window()
	{
		virtual ~Window() {}
		virtual void OnUpdate() = 0;

		static Window* Create();
	};
}