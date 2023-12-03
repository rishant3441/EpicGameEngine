cmake_minimum_required(VERSION 3.16)
project(EGELauncher)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(EGELauncher src/App.cpp src/LauncherLayer.cpp src/LauncherLayer.h)
target_include_directories(EGELauncher PUBLIC ${EGE_INCLUDE_DIR})
target_link_directories(EGELauncher PRIVATE ${EGE_LIB_DIR})
target_link_libraries(EGELauncher PRIVATE EpicGameEngine)
target_link_libraries(EGELauncher PRIVATE SDL2::SDL2 SDL2::SDL2main)

if (MSVC)
	add_custom_command(
		TARGET EGELauncher POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:EpicGameEngine-Scripting>" "${PROJECT_SOURCE_DIR}/src/Project/"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_LINKER_FILE:EpicGameEngine-Scripting>" "${PROJECT_SOURCE_DIR}/src/Project/"
	)
endif()