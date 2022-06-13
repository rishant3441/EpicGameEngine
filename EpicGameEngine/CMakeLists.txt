set(EGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include CACHE INTERNAL "Directory of EGE Header Files")
set(EGE_LIB_DIR ${PROJECT_BINARY_DIR} CACHE INTERNAL "Directory of Epic Games Engine Library Files")
set(EGE_LIB EpicGameEngine CACHE INTERNAL "Library File")

add_subdirectory(dependencies)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_library(
	EpicGameEngine SHARED 
	src/EpicGameEngine/Application.cpp
 "include/EpicGameEngine/Window/Window.h" "include/EpicGameEngine/ege_pch.h" "include/EpicGameEngine/Window/WindowsWindow.h" "src/EpicGameEngine/Window/WindowsWindow.cpp" "include/EpicGameEngine/Input/Input.h" "include/EpicGameEngine/Input/MouseCodes.h" "src/EpicGameEngine/Input/WindowsInput.cpp" "include/EpicGameEngine/Events/Event.h")

target_include_directories(
	EpicGameEngine PUBLIC
	include/
	PUBLIC
	${SDL2_INCLUDE_DIRS}
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

target_link_libraries(EpicGameEngine PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(EpicGameEngine PRIVATE spdlog::spdlog)

if (MSVC)
	target_compile_definitions(
		EpicGameEngine 
		PUBLIC
		EGE_PLATFORM_WINDOWS
		PRIVATE
		EGE_BUILD_DLL
	)
endif()
