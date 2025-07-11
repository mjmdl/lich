add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(stb)

set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

set(TL_EXPECTED_BUILD_TESTS OFF CACHE BOOL "Disable tests in tl::expected" FORCE)
add_subdirectory(expected)

set(SPDLOG_BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(spdlog)
target_compile_definitions(
	spdlog PRIVATE
	SPDLOG_COMPILED_LIB=1
	SPDLOG_LEVEL_NAMES={\"TRACE\",\"DEBUG\",\"INFO\",\"WARN\",\"ERROR\",\"FATAL\",\"OFF\"}
	SPDLOG_SHORT_LEVEL_NAMES={\"T\",\"D\",\"I\",\"W\",\"E\",\"F\",\"O\"}
	SPDLOG_DISABLE_DEFAULT_LOGGER=1
)
