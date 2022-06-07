set(EGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include CACHE INTERNAL "Directory of EGE Header Files")
set(EGE_LIB_DIR ${PROJECT_BINARY_DIR} CACHE INTERNAL "Directory of Epic Games Engine Library Files")
set(EGE_LIB EpicGameEngine CACHE INTERNAL "Library File")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_library(
	EpicGameEngine SHARED 
	src/EpicGameEngine/Application.cpp
 "include/EpicGameEngine/Window.h")

target_include_directories(
	EpicGameEngine PUBLIC
	include/
	PUBLIC
	${SDL2_INCLUDE_DIRS}
)

target_compile_definitions(
	EpicGameEngine PUBLIC
)

target_link_libraries(EpicGameEngine ${SDL2_LIBRARIES})

if (MSVC)
	target_compile_definitions(
		EpicGameEngine 
		PUBLIC
		EGE_PLATFORM_WINDOWS
		PRIVATE
		EGE_BUILD_DLL
	)
endif()
