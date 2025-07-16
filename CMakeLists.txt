cmake_minimum_required(VERSION 3.22)

project(
	lich
		DESCRIPTION "Lich Engine"
		VERSION "0.0.1"
		LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/binaries)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/binaries)

set(
	SOURCE_FILES
		source/lich/app.cpp
		source/lich/glfw_input.cpp
		source/lich/glfw_platform.cpp
		source/lich/glfw_window.cpp
		source/lich/imgui.cpp
		source/lich/layer.cpp
		source/lich/log.cpp
		source/lich/opengl_render.cpp
		source/lich/render.cpp
)	
set(
	HEADER_FILES
		source/lich/app.hpp
		source/lich/event.hpp
		source/lich/glfw_input.hpp
		source/lich/glfw_window.hpp
		source/lich/imgui.hpp
		source/lich/input.hpp
		source/lich/layer.hpp
		source/lich/log.hpp
		source/lich/opengl_render.hpp
		source/lich/platform.hpp
		source/lich/render.hpp
		source/lich/util.hpp
		source/lich/window.hpp
)
add_library(
	lich STATIC
		${SOURCE_FILES}
		${HEADER_FILES}
)

target_include_directories(
	lich
		PUBLIC source
		PRIVATE source/lich
)

target_precompile_headers(lich PUBLIC source/lich/pch.hpp)

target_compile_definitions(lich PRIVATE LICH_COMPILE_STEP=1)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(lich PRIVATE -Wall -Wextra -Wpedantic -g)
elseif(MSVC)
	target_compile_options(lich PRIVATE /W4)
endif()

add_subdirectory(vendor)
target_link_libraries(
	lich PUBLIC
		expected
		glad
		glfw
		glm
		imgui
		spdlog
		stb
)

add_subdirectory(sandbox)
