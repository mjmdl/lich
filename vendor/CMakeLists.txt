add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(spdlog)
add_subdirectory(stb)

target_compile_definitions(spdlog PRIVATE
	SPDLOG_COMPILED_LIB=1
	SPDLOG_LEVEL_NAMES={\"TRACE\",\"DEBUG\",\"INFO\",\"WARN\",\"ERROR\",\"FATAL\",\"OFF\"}
	SPDLOG_SHORT_LEVEL_NAMES={\"T\",\"D\",\"I\",\"W\",\"E\",\"F\",\"O\"}
	SPDLOG_DISABLE_DEFAULT_LOGGER=1)
