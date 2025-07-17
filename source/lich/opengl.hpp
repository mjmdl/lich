#ifndef LICH_OPENGL_HPP
#define LICH_OPENGL_HPP

#include <glad/glad.h>

#define GL_CHECK(...) \
	do { \
		pop_opengl_errors(); \
		__VA_ARGS__; \
		check_opengl_error(__FILE__, __LINE__); \
	} while (0)

#define GL_ASSERT(...) \
	do { \
		pop_opengl_errors(); \
		__VA_ARGS__; \
		check_opengl_error(__FILE__, __LINE__); \
	} while (0)

namespace lich {

enum class Shader_Data_Type;

void pop_opengl_errors();
void check_opengl_error(const char *file, int line);
void assert_opengl_error(const char *file, int line);
GLenum equivalent_opengl_type(Shader_Data_Type type);

}

#endif
