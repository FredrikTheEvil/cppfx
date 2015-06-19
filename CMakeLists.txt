# CMakeLists files in this project can
# refer to the root source directory of the project as ${cppfx_SOURCE_DIR} and
# to the root binary directory of the project as ${cppfx_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (cppfx)

include(CheckFunctionExists)
include(GenerateExportHeader)

set(cppfx_VERSION_MAJOR 0)
set(cppfx_VERSION_MINOR 1)
set(cppfx_VERSION_PATCH 0)
set(cppfx_VERSION_TRACK "alpha")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

option(BUILD_CPPFX_SHARED "Build Shared Library (/DLL)" OFF)
option(GLEW_STATIC "Link against static GLEW libraries" OFF)
option(GLFW_STATIC "Link against static GLFW libraries" ON)

if(BUILD_CPPFX_SHARED)
	set(cppfx_LIBRARY_TYPE SHARED)
else()
	set(cppfx_LIBRARY_TYPE STATIC)
endif()

check_function_exists(sprintf_s CPPFX_HAVE_SPRINTF_S)
check_function_exists(sscanf_s CPPFX_HAVE_SSCANF_S)
check_function_exists(fopen_s CPPFX_HAVE_FOPEN_S)

find_package(GLFW)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(png)

add_subdirectory (cppfx)
add_subdirectory (cppfx-sample)
