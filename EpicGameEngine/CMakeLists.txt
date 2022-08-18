set(EGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include CACHE INTERNAL "Directory of EGE Header Files")
set(EGE_LIB_DIR ${PROJECT_BINARY_DIR} CACHE INTERNAL "Directory of Epic Games Engine Library Files")
set(EGE_LIB EpicGameEngine CACHE INTERNAL "Library File")
set(mono_DIR dependencies/mono)

add_subdirectory(dependencies)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

find_package(glew REQUIRED)

find_package(mono REQUIRED)

find_package(OpenMP REQUIRED)

add_library(
	EpicGameEngine STATIC 
	src/EpicGameEngine/Application.cpp
 "include/EpicGameEngine/Window/Window.h" "include/EpicGameEngine/ege_pch.h" "include/EpicGameEngine/Window/WindowsWindow.h" "src/EpicGameEngine/Window/WindowsWindow.cpp" "include/EpicGameEngine/Input/Input.h" "include/EpicGameEngine/Input/MouseCodes.h" "src/EpicGameEngine/Input/WindowsInput.cpp" "include/EpicGameEngine/Events/Event.h" "src/EpicGameEngine/Events/EventConversion.cpp" "include/EpicGameEngine/Renderer/Renderer.h" "src/EpicGameEngine/Renderer/Renderer.cpp" "include/EpicGameEngine/Layers/Layer.h" "include/EpicGameEngine/Layers/LayerArray.h" "src/EpicGameEngine/Layers/LayerArray.cpp" "include/EpicGameEngine/GameObjects/GameObject.h" "src/EpicGameEngine/Layers/Layer.cpp" "include/EpicGameEngine/ImGui/ImGuiLayer.h" "src/EpicGameEngine/ImGui/ImGuiLayer.cpp" "include/EpicGameEngine/Renderer/Camera/CameraController.h" "src/EpicGameEngine/Renderer/Camera/CameraController.cpp" "include/EpicGameEngine/Timestep.h" src/EpicGameEngine/GameObjects/Scene.cpp include/EpicGameEngine/GameObjects/Scene.h src/EpicGameEngine/Renderer/Texture.cpp include/EpicGameEngine/Renderer/Texture.h include/EpicGameEngine/GameObjects/Components.h src/EpicGameEngine/GameObjects/GameObject.cpp src/EpicGameEngine/Scripting/ScriptingEngine.cpp include/EpicGameEngine/Scripting/ScriptingEngine.h src/EpicGameEngine/Scripting/ScriptingRegister.cpp include/EpicGameEngine/Scripting/ScriptingRegister.h  src/EpicGameEngine/Renderer/Camera/Camera.cpp include/EpicGameEngine/Renderer/Camera/Camera.h src/EpicGameEngine/Debug.cpp include/EpicGameEngine/Debug.h dependencies/dear_spdlog/dear_sink.cpp dependencies/dear_spdlog/dear_sink.h src/EpicGameEngine/GameObjects/SceneSerializer.cpp include/EpicGameEngine/GameObjects/SceneSerializer.h include/EpicGameEngine/PlatformUtils.h include/Platform/Windows/WindowsPlatformUtils.cpp src/EpicGameEngine/UUID.cpp include/EpicGameEngine/UUID.h)

target_include_directories(
	EpicGameEngine PUBLIC
	include/
	PUBLIC
	${SDL2_INCLUDE_DIRS}
	PUBLIC 
	dependencies/spdlog/include/
	PUBLIC
	dependencies/sdl-gpu/include/
	PUBLIC
	${OPENGL_INCLUDE_DIRS}
	PUBLIC
	dependencies/imgui/include/
	PUBLIC
	${GLEW_INCLUDE_DIR}
  PUBLIC
	dependencies/glm/
	PUBLIC
	dependencies/entt/include
	PUBLIC
	dependencies/dear_spdlog/
	PUBLIC
	dependencies/yaml-cpp/include/
	PUBLIC
	dependencies/ImGuizmo/
  PUBLIC
  ${MONO_INCLUDE_DIR}
)

include_directories(
	include/
)

target_compile_definitions(
	EpicGameEngine PUBLIC
)

target_precompile_headers(
	EpicGameEngine PUBLIC "include/EpicGameEngine/ege_pch.h"
)

add_dependencies(EpicGameEngine IMGUI)

target_link_libraries(EpicGameEngine PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(EpicGameEngine PRIVATE spdlog::spdlog)
target_link_libraries(EpicGameEngine PUBLIC SDL_gpu)
target_link_directories(EpicGameEngine PUBLIC opengl32)
target_link_libraries(EpicGameEngine PUBLIC IMGUI)
target_link_libraries(EpicGameEngine PUBLIC ${GLEW_GPU_LIBRARY_PATH})
target_link_libraries(EpicGameEngine PUBLIC yaml-cpp)
target_link_libraries(EpicGameEngine PUBLIC ImGuizmo)
target_link_libraries(EpicGameEngine PUBLIC ${MONO_LIBRARY})
target_link_libraries(EpicGameEngine PUBLIC OpenMP::OpenMP_CXX)
if (MSVC)
    target_link_libraries(EpicGameEngine PUBLIC Ws2_32.lib)
    target_link_libraries(EpicGameEngine PUBLIC Winmm.lib)
    target_link_libraries(EpicGameEngine PUBLIC Version.lib)
    target_link_libraries(EpicGameEngine PUBLIC Bcrypt.lib)
endif()

if (MSVC)
	target_compile_definitions(
		EpicGameEngine 
		PUBLIC
		EGE_PLATFORM_WINDOWS
	)
endif()

