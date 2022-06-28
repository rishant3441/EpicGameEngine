#include <EpicGameEngine/ImGui/ImGuiLayer.h>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Window/WindowsWindow.h>
#include <EpicGameEngine/Application.h>
#include <EpicGameEngine/Renderer/Renderer.h>
#include <spdlog/spdlog.h>

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

namespace EpicGameEngine
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {

    }
    ImGuiLayer::~ImGuiLayer()
    {

    }
    void ImGuiLayer::OnAttach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGui_ImplSDL2_InitForOpenGL(WindowsWindow::window, SDL_GL_GetCurrentContext());
        ImGui_ImplOpenGL3_Init("#version 120");
    }
    void ImGuiLayer::OnDetach()
    {

    }
    void ImGuiLayer::OnUpdate()
    {
        ImGui_ImplSDL2_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();


        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    void ImGuiLayer::OnEvent(std::shared_ptr<Event> e)
    {

    }
}