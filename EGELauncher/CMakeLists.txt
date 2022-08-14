cmake_minimum_required(VERSION 3.16)
project(EGELauncher)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(EGELauncher src/App.cpp src/LauncherLayer.cpp src/LauncherLayer.h)
target_include_directories(EGELauncher PUBLIC ${EGE_INCLUDE_DIR})
target_link_directories(EGELauncher PRIVATE ${EGE_LIB_DIR})
target_link_libraries(EGELauncher PRIVATE EpicGameEngine)
target_link_libraries(EGELauncher PUBLIC ${SDL2_LIBRARIES})
