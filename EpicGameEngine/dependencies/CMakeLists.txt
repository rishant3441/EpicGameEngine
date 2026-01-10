cmake_minimum_required(VERSION 3.16)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
add_library(IMGUI STATIC stb_image/stb_image.h stb_image/stb_image.cpp)

list(APPEND CMAKE_PREFIX_PATH "$<TARGET_FILE_DIR:SDL2::SDL2>")

add_subdirectory(SDL2)

target_sources(IMGUI PRIVATE
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(IMGUI PUBLIC ${IMGUI_DIR} PUBLIC ${IMGUI_DIR}/backends PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(IMGUI PUBLIC SDL2::SDL2 SDL2::SDL2main)

add_subdirectory(glew-cmake/)
add_subdirectory(spdlog)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Building shared libs is off")
add_subdirectory(yaml-cpp)
add_subdirectory(ImGuizmo)
add_subdirectory(assimp)

# add_subdirectory(sdl-gpu)
set(GLEW_USE_STATIC_LIBS ON)
