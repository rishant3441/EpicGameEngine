# EpicGameEngine
A small Epic 2D game engine! Windows only currently.
## Disclaimer
This is not related to Epic Games in any way shape or form. You should also not use this engine as it is not in a (what I deem to be) usable state. 

The below is AI-generated as AI is pretty good at that type of thing - Github Copilot + Gemini 3 Pro Preview
## Getting Started (AI Generated)

### Cloning
To clone the repository, make sure to include submodules:
```bash
git clone --recursive github.com/rishant3441/EpicGameEngine.git
```

### Building the Project
1. Navigate to the `scripts/` directory.
2. Run `Win-GenProjects.bat` to generate the Visual Studio solution.
3. Open `EpicGameEngine.sln` located in the `build/` folder.
4. Build the solution (specifically `EGELauncher` and `EpicGameEditor`).

### Workflow
1. **Create Project**: Run the compiled `EGELauncher` executable to create a new game project.
2. **Generate & Build Game Scripts**: 
   - Navigate to your newly created project folder.
   - Run `Win-GenScripts.bat`. This generates a Visual Studio solution in a `build/` subdirectory.
   - Open the generated solution (e.g., `build/ScriptProject.sln`) and **Build** it. This compiles your C# scripts and places the resulting DLL (`ScriptProject.dll`) into your project folder.
3. **Configure Editor Path**: In `EGELauncher`, open the **Settings** menu and select the `EpicGameEditor` executable using the file picker. This is required to launch projects.
4. **Enter Editor**: Use the `EGELauncher` to launch the `EpicGameEditor` with your new project.