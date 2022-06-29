cmake_minimum_required(VERSION 3.16)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
add_library(IMGUI STATIC)

find_package(SDL2 REQUIRED)

target_sources(IMGUI PRIVATE
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
)

target_include_directories(IMGUI PUBLIC ${IMGUI_DIR} PUBLIC ${IMGUI_DIR}/backends PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(IMGUI PUBLIC ${SDL2_LIBRARIES})

add_subdirectory(spdlog)
