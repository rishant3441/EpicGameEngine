#include <EpicGameEngine.h>
#include <filesystem>

namespace EpicGameEngine
{
    struct ProjectData
    {
    public:
        ProjectData(const std::string& projectFile, std::string  dir, const std::string& name)
                : projectFilePath(projectFile), workingDir(std::move(dir)), Name(name)
        {

        }
        ProjectData() = default;
        ~ProjectData() = default;

        std::string Name;
        std::string lastModified;
        std::filesystem::path projectFilePath;
        std::string workingDir;
    };

	class LauncherLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnUpdate(Timestep time) override;
		void OnImGuiRender() override;
		void OnRender() override;

		std::string executablePath;
		std::vector<ProjectData> projects;
	private:
	    bool showSettings = false;
	    bool showAddProject = false;

	    void AddProject();
	    void NewProject();
	    void Settings();
	    void Projects();
	};
}