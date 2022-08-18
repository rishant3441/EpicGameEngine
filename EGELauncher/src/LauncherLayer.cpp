#include "LauncherLayer.h"
#include <EpicGameEngine.h>
#include <EpicGameEngine/PlatformUtils.h>
#include <imgui.h>
#include <cstdlib>
#include <chrono>

namespace EpicGameEngine
{
    LauncherLayer::LauncherLayer(int argc, char** argv)
    {
        argCount = argc;
        args = *argv;
    }

	void LauncherLayer::OnAttach()
	{
	    spdlog::info("{}", "");
        // TODO: Deserialize cached data about projects
	}
    void LauncherLayer::OnUpdate(Timestep time)
    {

    }
    void LauncherLayer::OnImGuiRender()
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
                if (ImGui::MenuItem("Settings"))
                {
                    if (showSettings)
                        showSettings = false;
                    else
                        showSettings = true;
                }
                if (ImGui::MenuItem("New Project"))
                {
                    if (showNewProject)
                        showNewProject = false;
                    else
                        showNewProject = true;
                }
                if (ImGui::MenuItem("Add Project"))
                {
                    if (showAddProject)
                        showAddProject = false;
                    else
                        showAddProject = true;
                }
                ImGui::EndMenuBar();
            }

            Projects();

            if (showSettings)
                Settings();

            if (showNewProject)
                NewProject();

            if (showAddProject)
                AddProject();

            static bool demo = true;
            ImGui::ShowDemoWindow(&demo);

            ImGui::End();
        }
    }
    void LauncherLayer::OnRender()
    {

    }

    static ProjectData tempProject;

    void LauncherLayer::AddProject()
    {
        ImGui::Begin("Add Project", nullptr, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Exit"))
            {
                showAddProject = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 112.0f);
        ImGui::Text("Project Name");
        ImGui::NextColumn();

        static char nameBuffer[256];
        strcpy_s(nameBuffer, sizeof(nameBuffer), tempProject.Name.c_str());
        static char projectFileBuffer[256];
        strcpy_s(projectFileBuffer, sizeof(projectFileBuffer), tempProject.projectFilePath.string().c_str());
        if (ImGui::InputText("##projectName", nameBuffer, sizeof(nameBuffer)))
        {
            tempProject.Name = nameBuffer;
        }
        ImGui::Columns(1);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 112.0f);
        ImGui::Text("Project File Path");
        ImGui::NextColumn();

        if (ImGui::InputText("##projectFilePath", projectFileBuffer, sizeof(projectFileBuffer)))
        {
            tempProject.projectFilePath = projectFileBuffer;
        }


        if (ImGui::Button("Open..."))
        {
            std::string filepath = FileDialogs::OpenFile("Epic Game Engine Scene (*.ege)\0*.ege\0");

            if (filepath.empty())
                return;

            tempProject.projectFilePath = filepath;
            std::filesystem::path temp = tempProject.projectFilePath;
            std::string tempS = temp.remove_filename().string();
            tempProject.workingDir = tempS;
            tempProject.Name = tempProject.projectFilePath.filename().string();
        }

        if (ImGui::Button("Add Project"))
        {
            projects.emplace_back(tempProject);
        }

        ImGui::End();
    }

    static std::string dateTimeToString(std::filesystem::file_time_type tp)
    {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - std::filesystem::file_time_type::clock::now()
                + system_clock::now());
        std::time_t t = system_clock::to_time_t(sctp);
        std::tm *gmt = std::gmtime(&t);
        std::stringstream buffer;
        buffer << std::put_time(gmt, "%A, %d %B %Y %H:%M");
        std::string formattedFileTime = buffer.str();
        return formattedFileTime;
    }

    void LauncherLayer::NewProject()
    {
        ImGui::Begin("New Project", nullptr, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Exit"))
            {
                showNewProject = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 112.0f);
        ImGui::Text("Project Name");
        ImGui::NextColumn();

        static char nameBuffer[256];
        strcpy_s(nameBuffer, sizeof(nameBuffer), tempProject.Name.c_str());
        static char projectFileBuffer[256];
        strcpy_s(projectFileBuffer, sizeof(projectFileBuffer), tempProject.workingDir.c_str());
        if (ImGui::InputText("##projectName", nameBuffer, sizeof(nameBuffer)))
        {
            tempProject.Name = nameBuffer;
        }
        ImGui::Columns(1);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 112.0f);
        ImGui::Text("Project Folder");
        ImGui::NextColumn();

        if (ImGui::InputText("##projectFilePath", projectFileBuffer, sizeof(projectFileBuffer)))
        {
            tempProject.workingDir = projectFileBuffer;
        }

        if (ImGui::Button("Open..."))
        {
            std::string filepath = FileDialogs::OpenFolder("Epic Game Engine Scene (*.ege)\0*.ege\0");

            if (filepath.empty())
            {
                ImGui::End();
                return;
            }

            /*tempProject.projectFilePath = filepath;
            std::filesystem::path temp = tempProject.projectFilePath;
            std::string tempS = temp.remove_filename().string();
            tempProject.workingDir = tempS;
            tempProject.Name = tempProject.projectFilePath.filename().string();*/
            std::filesystem::path workingDir = filepath;
            workingDir = workingDir.lexically_normal();
            tempProject.workingDir = workingDir.string();
            std::filesystem::file_time_type time = std::filesystem::last_write_time(workingDir);
            tempProject.lastModified = dateTimeToString(time);

        }

        if (ImGui::Button("Create Project"))
        {
            // TODO: Implement Copy Code
            std::filesystem::path newWorkingDirName = tempProject.workingDir;
            newWorkingDirName = newWorkingDirName.parent_path().append(tempProject.Name);
            //spdlog::info("{}", newWorkingDirName.string());
            //tempProject.workingDir = newWorkingDirName.string();
            if (!std::filesystem::exists(tempProject.workingDir))
            {
                std::filesystem::create_directory(tempProject.workingDir);
            }
            std::filesystem::rename(tempProject.workingDir, newWorkingDirName);
            std::filesystem::copy("src/Project", newWorkingDirName, std::filesystem::copy_options::recursive);
            tempProject.workingDir = newWorkingDirName.string();
            tempProject.projectFilePath = newWorkingDirName.append("Project.ege");

            projects.emplace_back(tempProject);
        }

        ImGui::End();
    }

    // TODO FOR NEXT TIME:
    /*
     *
     * 1. Implement argument based code to pass in a game assembly file :check:
     * 2. Implement ScriptingEngine code that uses said data :check:
     * 3. Strip out all game-specific CSharp code from the ScriptCore :check:
     * 4. Copy new ScriptCore DLL to Copied Project Dir :check:
     * 5. Implement running the engine with new argument of the game dll
     * 6. In the future automate building the game DLL within the engine
     *
     */

    void LauncherLayer::Settings()
    {
	    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Exit"))
            {
                showSettings = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        static char buffer[256];
        strcpy_s(buffer, sizeof(buffer), executablePath.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 112.0f);
        ImGui::Text("Executable Path");
        ImGui::NextColumn();
        if (ImGui::InputText("##executablePath", buffer, sizeof(buffer)))
        {
           executablePath = buffer;
        }
        ImGui::SameLine();
        if (ImGui::Button("Open..."))
        {
           executablePath = FileDialogs::OpenFile("Epic Game Engine Executable (*.exe)\0*.exe\0");
        }

        ImGui::End();
    }

    void LauncherLayer::Projects()
    {
        ImGui::Begin("Projects");

        if (ImGui::BeginTable("Projects", 3, ImGuiTableFlags_BordersOuter))
        {
            ImGui::TableSetupColumn("Project Name", ImGuiTableColumnFlags_NoResize);
            ImGui::TableSetupColumn("Last Modified", ImGuiTableColumnFlags_NoResize);
            ImGui::TableHeadersRow();
            for (auto & project : projects)
            {
                if (ImGui::TableNextColumn())
                {
                    ImGui::Text("%s", project.Name.c_str());
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.8f),"%s", project.workingDir.c_str());
                }
                ImGui::TableNextColumn();
                ImGui::Text("%s", project.lastModified.c_str());
                ImGui::TableNextColumn();
                ImGui::SameLine(ImGui::GetColumnWidth() - 45.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.5f, 13.0f));

                if (ImGui::Button("Open"))
                {
                    // TODO: Write code to open project
                    std::filesystem::path workingDirPath = project.workingDir;
                    workingDirPath = workingDirPath.lexically_normal();
                    std::filesystem::path executableFSPath = executablePath;
                    executableFSPath = executableFSPath.lexically_normal();

                    // TODO: Make platform-agnostic
                    std::filesystem::path dllPath = workingDirPath;
                    dllPath = dllPath.append("ScriptProject.dll").lexically_normal().string();
                    std::string command = "cd " + workingDirPath.string() + " && " + executableFSPath.string() + " " + project.projectFilePath.lexically_normal().string() + " " + dllPath.string();
                    spdlog::info(command);
                    std::system(command.c_str());
                }
                ImGui::PopStyleVar();

                ImGui::TableNextRow();
            }
            ImGui::EndTable();
        }

        ImGui::End();
    }
}
