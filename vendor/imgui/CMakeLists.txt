cmake_minimum_required(VERSION 3.22)

project(
	imgui
		DESCRIPTION "Dear ImGui: Bloat-free Graphical User interface for C++ with minimal dependencies."
		VERSION "1.92.0"
		LANGUAGES CXX
)

add_library(
	imgui STATIC
		imgui/imgui.cpp
    	imgui/imgui_demo.cpp
    	imgui/imgui_draw.cpp
    	imgui/imgui_tables.cpp
    	imgui/imgui_widgets.cpp
    	imgui/backends/imgui_impl_glfw.cpp
    	imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC . imgui)
