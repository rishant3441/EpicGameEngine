cmake_minimum_required(VERSION 3.18)
project(ScriptProject VERSION 0.1.0 LANGUAGES CSharp)
include(CSharpUtilities)
file(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cs)
add_library(ScriptProject SHARED ${SRC_FILES})
set_property(TARGET ScriptProject PROPERTY VS_DOTNET_REFERENCES EpicGameEngine-Scripting)
set_property(TARGET ScriptProject PROPERTY VS_DOTNET_REFERENCE_EpicGameEngine-Scripting "../../EpicGameEngine-Scripting.dll")

add_custom_command(
	TARGET ScriptProject POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:ScriptProject>" "${PROJECT_SOURCE_DIR}/../../"
)