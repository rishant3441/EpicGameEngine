#include <EpicGameEngine/Renderer/Camera/CameraController.h>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Renderer/Renderer.h>

namespace EpicGameEngine
{
	void CameraController::CreateCamera()
	{
		GPU_EnableCamera(Renderer::target, true);
	}

	void CameraController::UpdateCamera()
	{
		CameraController::camera = &(Renderer::GetTarget()->camera);
	}

	void CameraController::DestroyCamera()
	{
		GPU_EnableCamera(Renderer::target, false);
	}
}