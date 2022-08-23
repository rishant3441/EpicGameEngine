cmake_minimum_required(VERSION 3.16)
project (EpicGameEngine)

set (CMAKE_CXX_STANDARD 20)
if (MSVC)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif()

add_subdirectory(scripts)
add_subdirectory(EpicGameEngine)
add_subdirectory(EpicGameEditor)
add_subdirectory(EpicGameEngine-Scripting)
add_subdirectory(EGELauncher)
add_subdirectory(Sandbox)