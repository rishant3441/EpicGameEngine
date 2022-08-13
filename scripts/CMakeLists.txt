cmake_minimum_required(VERSION 3.18)
project(CMakeUtility)

if (MSVC)

add_custom_target(CMakeUtility ALL)

add_custom_command(TARGET CMakeUtility COMMAND
	"${CMAKE_CURRENT_LIST_DIR}\\Win-GenProjects.bat"
) 

endif()