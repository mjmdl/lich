cmake_minimum_required(VERSION 3.22)

project(lich
	DESCRIPTION "Lich Engine"
	VERSION "0.0.1"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/binaries)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/binaries)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
	add_compile_options(/W4)
endif()

add_library(lich STATIC
	source/lich/app.cpp
	source/lich/log.cpp)

target_include_directories(lich
	PUBLIC source
	PRIVATE source/lich)

target_precompile_headers(lich PUBLIC source/lich/pch.hpp)

add_subdirectory(vendor/spdlog)
target_compile_definitions(spdlog PRIVATE
	SPDLOG_COMPILED_LIB=1
	SPDLOG_LEVEL_NAMES={\"TRACE\",\"DEBUG\",\"INFO\",\"WARN\",\"ERROR\",\"FATAL\",\"OFF\"}
	SPDLOG_SHORT_LEVEL_NAMES={\"T\",\"D\",\"I\",\"W\",\"E\",\"F\",\"O\"}
	SPDLOG_DISABLE_DEFAULT_LOGGER=1)

target_link_libraries(lich PUBLIC
	spdlog)

add_subdirectory(sandbox)
