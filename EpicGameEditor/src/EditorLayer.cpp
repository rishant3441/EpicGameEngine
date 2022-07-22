#include "EditorLayer.h"
#include <imgui.h>
#include <SDL_gpu_OpenGL_4.h>
#include <cassert>

namespace EpicGameEngine
{
	void EditorLayer::OnAttach()
	{
        activeScene = std::make_shared<EpicGameEngine::Scene>();
        activeScene->viewportSize.x = 1024;
        activeScene->viewportSize.y = 576;
		Renderer::ToggleDrawingToTexture();
		SDL_Color color;
		color.a = 255;
		color.r = 255;
		color.g = 0;
		color.b = 0;

        rect = activeScene->CreateGameObject("Rectangle");
        rect.AddComponent<EpicGameEngine::SpriteRendererComponent>(color);
        rect.GetComponent<EpicGameEngine::TransformComponent>().Position.x = 462;
        rect.GetComponent<EpicGameEngine::TransformComponent>().Position.y = 238;

        class Rectangle : public EpicGameEngine::ScriptableGameObject
        {
        public:
            void OnCreate()
            {

            }

            void OnDestroy()
            {

            }

            void OnUpdate(Timestep ts)
            {
               //spdlog::info("Timestep: {}", ts.GetSeconds());
            }
        };

        rect.AddComponent<EpicGameEngine::NativeScriptComponent>().Bind<Rectangle>();

        camera = activeScene->CreateGameObject();
        camera.AddComponent<EpicGameEngine::CameraComponent>();
        auto& cameraTransform = camera.GetComponent<EpicGameEngine::TransformComponent>();
        auto& cc = camera.GetComponent<EpicGameEngine::CameraComponent>();
        cameraTransform.Position.z = -1;
        camera.GetComponent<EpicGameEngine::CameraComponent>().Camera.SetViewportSize(1024, 576);
        class CameraContrller : public ScriptableGameObject
        {

            void OnUpdate(Timestep time)
            {
                auto& cameraTransform = camera.GetComponent<TransformComponent>();

                if (Input::isKeyPressed(Keyboard::W))
                {
                    cameraTransform.Position.y -= 300 * time.GetSeconds();
                }

                if (Input::isKeyPressed(Keyboard::A))
                {
                    cameraTransform.Position.x -= 300 * time.GetSeconds();
                }

                if (Input::isKeyPressed(Keyboard::S))
                {
                    cameraTransform.Position.y += 300 * time.GetSeconds();
                }

                if (Input::isKeyPressed(Keyboard::D))
                {
                    cameraTransform.Position.x += 300 * time.GetSeconds();
                }
            }

            void OnStart()
            {

            }
        };
        camera.AddComponent<NativeScriptComponent>().Bind<CameraContrller>();

        // Panels
        gameObjectsPanel.SetContext(activeScene);
	}

	EditorLayer::~EditorLayer()
	{
		GPU_FreeImage(Renderer::texture);
	}

	void EditorLayer::OnUpdate(Timestep time)
    {

        activeScene->OnUpdate(time);

        if (activeScene->viewportSize.x != Renderer::target->w || activeScene->viewportSize.y != Renderer::target->h)
        {
            //activeScene->OnViewportResize(Renderer::target->w, Renderer::target->h);
        }
	}
	void EditorLayer::OnImGuiRender()
	{
		static bool dockingEnabled = true;
		if (dockingEnabled)
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags;
			//dockspace_flags |= ImGuiDockNodeFlags_NoResize;


			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar; // ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			//io.ConfigFlags ^= ImGuiConfigFlags_ViewportsEnable;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                    if (ImGui::MenuItem("Exit")) Application::Get().Close();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
			

			ImGui::Begin("Viewport");
			void* textureID = (void*) GPU_GetTextureHandle(Renderer::texture);
			ImGui::Image(textureID, ImVec2{ static_cast<float>(Renderer::target->w), static_cast<float>(Renderer::target->h) });
			ImGui::End();

			gameObjectsPanel.OnImGuiRender();

            ImGui::Begin("Debug Log");
            ImGui::End();

			ImGui::End();
		}
	}

    void EditorLayer::OnRender()
    {
    }
}