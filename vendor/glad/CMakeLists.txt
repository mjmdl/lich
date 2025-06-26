cmake_minimum_required(VERSION 3.22)

project(glad
	DESCRIPTION "Multi-Language Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs."
	VERSION "2.0.8"
	LANGUAGES C CXX)

add_library(glad STATIC glad/src/glad.c)

target_include_directories(glad PUBLIC glad/include)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_link_libraries(glad PUBLIC GL)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(glad PUBLIC opengl32)
else()
	message(FATAL_ERROR "Platform OpenGL library name is not defined.")
endif()
