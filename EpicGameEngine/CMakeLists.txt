set(EGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include CACHE INTERNAL "Directory of EGE Header Files")
set(EGE_LIB_DIR ${PROJECT_BINARY_DIR} CACHE INTERNAL "Directory of Epic Games Engine Library Files")
set(EGE_LIB EpicGameEngine CACHE INTERNAL "Library File")

add_subdirectory(dependencies)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package (SDL_gpu REQUIRED)

find_package(OpenGL REQUIRED)

find_package(glew REQUIRED)

add_library(
	EpicGameEngine STATIC 
	src/EpicGameEngine/Application.cpp
 "include/EpicGameEngine/Window/Window.h" "include/EpicGameEngine/ege_pch.h" "include/EpicGameEngine/Window/WindowsWindow.h" "src/EpicGameEngine/Window/WindowsWindow.cpp" "include/EpicGameEngine/Input/Input.h" "include/EpicGameEngine/Input/MouseCodes.h" "src/EpicGameEngine/Input/WindowsInput.cpp" "include/EpicGameEngine/Events/Event.h" "src/EpicGameEngine/Events/EventConversion.cpp" "include/EpicGameEngine/Renderer/Renderer.h" "src/EpicGameEngine/Renderer/Renderer.cpp" "include/EpicGameEngine/Layers/Layer.h" "include/EpicGameEngine/Layers/LayerArray.h" "src/EpicGameEngine/Layers/LayerArray.cpp" "include/EpicGameEngine/GameObjects/GameObject.h" "src/EpicGameEngine/GameObjects/Rectangle.cpp" "src/EpicGameEngine/Layers/Layer.cpp" "include/EpicGameEngine/ImGui/ImGuiLayer.h" "src/EpicGameEngine/ImGui/ImGuiLayer.cpp" "include/EpicGameEngine/Renderer/Camera/CameraController.h" "src/EpicGameEngine/Renderer/Camera/CameraController.cpp" "include/EpicGameEngine/Timestep.h")

target_include_directories(
	EpicGameEngine PUBLIC
	include/
	PUBLIC
	${SDL2_INCLUDE_DIRS}
	PUBLIC 
	dependencies/spdlog/include/
	PUBLIC
	${SDL2_GPU_INCLUDE_DIR}
	PUBLIC
	${OPENGL_INCLUDE_DIRS}
	PUBLIC
	dependencies/imgui/include/
	PUBLIC
	${GLEW_INCLUDE_DIR}
)

include_directories(
	include/
)

target_compile_definitions(
	EpicGameEngine PUBLIC
)

target_precompile_headers(
	EpicGameEngine PRIVATE "include/EpicGameEngine/ege_pch.h"
)

add_dependencies(EpicGameEngine IMGUI)

target_link_libraries(EpicGameEngine PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(EpicGameEngine PRIVATE spdlog::spdlog)
target_link_libraries(EpicGameEngine PUBLIC ${SDL2_GPU_LIBRARY_PATH})
target_link_directories(EpicGameEngine PUBLIC opengl32)
target_link_libraries(EpicGameEngine PUBLIC IMGUI)
target_link_libraries(EpicGameEngine PUBLIC ${GLEW_GPU_LIBRARY_PATH})

if (MSVC)
	target_compile_definitions(
		EpicGameEngine 
		PUBLIC
		EGE_PLATFORM_WINDOWS
	)
endif()
