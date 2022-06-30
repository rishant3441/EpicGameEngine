#pragma once
#include <EpicGameEngine/ege_pch.h>

namespace EpicGameEngine
{
	class EPICGAMEENGINE_API Timestep
	{
	public:
		Timestep(float time)
			: time(time)
		{
		}

		float GetSeconds() const { return time; }
		float GetMilliseconds() const { return time * 1000.0f; }
	private:
		float time; 
	};
}
