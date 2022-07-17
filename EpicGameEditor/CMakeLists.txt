cmake_minimum_required(VERSION 3.16)
project(EpicGameEditor)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(EpicGameEditor src/EpicGameEditor.cpp "src/EditorLayer.h" "src/EditorLayer.cpp")
target_include_directories(EpicGameEditor PUBLIC ${EGE_INCLUDE_DIR})
target_link_directories(EpicGameEditor PRIVATE ${EGE_LIB_DIR})
target_link_libraries(EpicGameEditor PRIVATE EpicGameEngine)
target_link_libraries(EpicGameEditor PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(EpicGameEditor PUBLIC EpicGameEngine-Scripting)

if (MSVC)
	add_custom_command(
		TARGET EpicGameEditor POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:EpicGameEngine-Scripting>" "${PROJECT_SOURCE_DIR}"
	)
endif()