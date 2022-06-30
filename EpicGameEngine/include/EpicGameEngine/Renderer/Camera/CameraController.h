#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Core.h>
#include <EpicGameEngine/Renderer/Renderer.h>
#include <SDL_gpu.h>

namespace EpicGameEngine
{
	class EPICGAMEENGINE_API CameraController
	{
	public:
		CameraController() = default;
		~CameraController() = default;

		static void CreateCamera();
		static void UpdateCamera();
		static void DestroyCamera();
		static GPU_Camera* GetCamera() { return camera; }

		inline static GPU_Camera* camera;
	};
}
