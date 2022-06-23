cmake_minimum_required(VERSION 3.16)
project(EpicGameEngineSandbox)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(Demo src/SandboxApp.cpp src/ExampleLayer.h src/ExampleLayer.cpp)
target_include_directories(Demo PUBLIC ${EGE_INCLUDE_DIR})
target_link_directories(Demo PRIVATE ${EGE_LIB_DIR})
target_link_libraries(Demo PRIVATE EpicGameEngine)
target_link_libraries(Demo PUBLIC ${SDL2_LIBRARIES})

if (MSVC)
	add_custom_command(
		TARGET Demo POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:EpicGameEngine>" "$<TARGET_FILE_DIR:Demo>"
	)
endif()