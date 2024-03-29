#include "EditorLayer.h"
#include <imgui.h>
#include <cassert>

#include <EpicGameEngine/PlatformUtils.h>
#include <EpicGameEngine/Scripting/ScriptingEngine.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Debug.h>


namespace EpicGameEngine
{
    static glm::mat4 cubeTransform;
    void EditorLayer::OnAttach()
	{
        // Initializes scripting engine with args
        if (argCount > 2)
        {
            Debug::Log::LogInfo("{}", args[2]);
            ScriptingEngine::Init(argCount, args);
        }
        else
        {
            ScriptingEngine::Init();
        }
        // Activates logger
	    ssink = dear_sink_mt();

        // Creates editor camera and scene
        activeScene = std::make_shared<EpicGameEngine::Scene>();
        editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);


// Testing Example
#if 0
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

        camera = activeScene->CreateGameObject("Camera");
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
#endif

        // Panels
        gameObjectsPanel.SetContext(activeScene);
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnUpdate(Timestep time)
    {
        // Updates camera viewport if the size changes
        if (FramebufferSpecification spec = frameBuffer->GetSpecification(); viewportSize.x > 0.0f && viewportSize.y > 0.0f &&
                                                                             (spec.Width != viewportSize.x || spec.Height != viewportSize.y))
        {
            frameBuffer->Resize((uint32_t) viewportSize.x, (uint32_t) viewportSize.y);
            editorCamera.SetViewportSize(viewportSize.x, viewportSize.y);
        }

        frameBuffer->Bind();
        Renderer::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f});
        Renderer::Clear();

        frameBuffer->ClearAttachment(1, -1);

        // Renders based on SceneState
        switch (sceneState)
        {
            case SceneState::Edit:
                editorCamera.OnUpdate(time);
                activeScene->OnEditorUpdate(time, editorCamera);
                break;
            case SceneState::Play:
                activeScene->OnRuntimeUpdate(time);
                break;
        }

        frameBuffer->Unbind();
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
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

			}

            style.WindowMinSize.x = minWinSizeX;


			if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // TODO: Implement Keyboard Shortcuts

                    if (ImGui::MenuItem("New", "Ctrl+N"))
                    {
                        activeScene = std::make_shared<Scene>();
                        activeScene->OnViewportResize(viewportSize.x, viewportSize.y);
                        gameObjectsPanel.SetContext(activeScene);

                        spdlog::info("EDITOR: New project created");
                    }

                    if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    {
                         std::string filepath = FileDialogs::OpenFile("epic game engine scene (*.ege)\0*.ege\0");

                         if (!filepath.empty())
                         {
                             activeScene = std::make_shared<Scene>();
                             activeScene->OnViewportResize(viewportSize.x, viewportSize.y);
                             gameObjectsPanel.SetContext(activeScene);

                             SceneSerializer serializer(activeScene);
                             serializer.Deserialize(filepath);
                         }
                    }

                    if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    {

                        std::string filepath = FileDialogs::SaveFile("Epic Game Engine Scene (*.ege)\0*.ege\0");
                        if (!filepath.empty())
                        {
                            SceneSerializer serializer(activeScene);
                            serializer.Serialize(filepath);
                        }
                    }

                    if (ImGui::MenuItem("Exit")) Application::Get().Close();

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
			
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f});
			// TODO: Disallow focusing the viewport and hovering effects
			ImGui::Begin("Viewport");
			uint64_t textureID = frameBuffer->GetColorAttachmentRendererID();
			ImVec2 imGuiViewportSize = ImGui::GetContentRegionAvail();
			viewportSize = { imGuiViewportSize.x, imGuiViewportSize.y };
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ static_cast<float>(viewportSize.x), static_cast<float>(viewportSize.y) }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			GameObject selectedGameObject = gameObjectsPanel.GetSelectedGameObject();
            if (selectedGameObject.operator bool())
			{
			    ImGuizmo::SetOrthographic(false);
			    ImGuizmo::SetDrawlist();
			    float windowWidth = (float) ImGui::GetWindowWidth();
			    float windowHeight = (float) ImGui::GetWindowHeight();
			    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			    /*auto cameraGameObject = activeScene->GetPrimaryCamera();
			    const auto& cameraC = cameraGameObject.GetComponent<CameraComponent>();
			    glm::mat4 cameraView = glm::inverse(cameraGameObject.GetComponent<TransformComponent>().GetTransform());
			    auto cameraTransform = cameraGameObject.GetComponent<TransformComponent>();
			    glm::mat4 cameraProjection = glm::perspective(glm::radians(cameraC.Camera.perspectiveVerticalFOV), cameraC.Camera.aspectRatio, cameraC.Camera.perspectiveNear, cameraC.Camera.perspectiveFar);
*/
			    const glm::mat4& cameraProjection = editorCamera.GetProjectionMatrix();
			    const glm::mat4& cameraView = editorCamera.GetViewMatrix();

                auto& tc = selectedGameObject.GetComponent<TransformComponent>();
                glm::mat4 transform = tc.GetTransform();


                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(transform));
                //ImGuizmo::ViewManipulate(glm::value_ptr(cameraView), 8.0f, ImVec2(0, 0), ImVec2(10, 10), ImColor(255, 0, 0, 255));
                glm::vec3 Position = { 0, 0, -2 };
                glm::vec3 Rotation = { 0, 0, 0 };
                glm::vec3 Scale = { 1000, 1000, 1000 };
                cubeTransform = glm::translate(glm::mat4{1.0f}, Position) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4{1.0f}, Scale);
                //ImGuizmo::DrawCubes(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(cubeTransform), 1);

			    if (ImGuizmo::IsUsing())
			    {
			        tc.Position = (glm::vec3(transform[3]));
			    }

			}

			ImGui::End();
            ImGui::PopStyleVar();

            ImGui::Begin("Content Browser");
            ImGui::Text("Epic Content Browser"); // TODO: Create content browser
            ImGui::End();

			Toolbar();

            ssink->draw_imgui(); // Log Window


			gameObjectsPanel.OnImGuiRender();



			ImGui::End();
		}
	}

    void EditorLayer::OnRender()
    {
    }

    void EditorLayer::OnEvent(std::shared_ptr<Event> e)
    {
        editorCamera.OnEvent(e);
    }

    void EditorLayer::Toolbar()
    {

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

        ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        float size = ImGui::GetWindowHeight() - 4.0f;
        Ref<Texture> icon = (sceneState == SceneState::Edit) ? playIcon : stopIcon;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size - 0.5f));
        if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)))
        {
            if (sceneState == SceneState::Edit)
            {
                OnScenePlay();
            }
            else if (sceneState == SceneState::Play)
            {
                OnSceneStop();
            }
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::End();
    }

    void EditorLayer::DefferedOnAttach()
    {
        // Sets camera viewport
        FramebufferSpecification spec;
        spec.Width = 1024;
        spec.Height = 576;
        spec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        frameBuffer = Framebuffer::Create(spec);

        editorCamera.SetViewportSize(1024, 576);

        playIcon = Texture2D::Create("resources/icons/PlayButton.png");
        stopIcon = Texture2D::Create("resources/icons/StopButton.png");
        // Loads images here due to the
        // renderer not being initialized
        // Serializes any scene passed in as an argument
        if (argCount > 1)
        {
            activeScene = std::make_shared<Scene>();
            activeScene->OnViewportResize(viewportSize.x, viewportSize.y);
            gameObjectsPanel.SetContext(activeScene);

            SceneSerializer serializer(activeScene);
            std::string filepath = args[1];
            serializer.Deserialize(filepath);
        }

    }

    void EditorLayer::OnScenePlay()
    {
        sceneState = SceneState::Play;

        activeScene->OnRuntimeStart();
    }

    void EditorLayer::OnSceneStop()
    {
        sceneState = SceneState::Edit;

        activeScene->OnRuntimeStop();
    }
}
