#include <glad/glad.h>
#include <tl/expected.hpp>

#include "log.hpp"
#include "renderer.hpp"

namespace lich {

static tl::expected<GLuint, std::string>
compile_shader_type_(const GLchar *source, GLenum type) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		std::string log(static_cast<Usize>(length), '\0');
		glGetShaderInfoLog(shader, length, &length, log.data());
		glDeleteShader(shader);

		return tl::unexpected{log};
	}

	return shader;
}

Shader::Shader(const std::string &vertex_source, const std::string &fragment_source):
	_id{0} {
	
	const GLchar *source = reinterpret_cast<const GLchar *>(vertex_source.c_str());
	auto vertex = compile_shader_type_(source, GL_VERTEX_SHADER);
	if (!vertex) {
		log_error("Failed to compile a GLSL vertex shader: {}", vertex.error());
		LICH_ABORT();
	}

	source = reinterpret_cast<const GLchar *>(fragment_source.c_str());
	auto fragment = compile_shader_type_(source, GL_FRAGMENT_SHADER);
	if (!fragment) {
		log_error("Failed to compile a GLSL fragment shader: {}", fragment.error());
		glDeleteShader(vertex.value());
		LICH_ABORT();
	}
	
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex.value());
	glAttachShader(program, fragment.value());
	glLinkProgram(program);

	glDeleteShader(vertex.value());
	glDeleteShader(fragment.value());

	GLint status = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		std::string log(static_cast<Usize>(length), '\0');
		glGetProgramInfoLog(program, length, &length, log.data());
		glDeleteProgram(program);

		log_error("Failed to link a GLSL program: {}", log);
		LICH_ABORT();
	}

	_id = program;
}

Shader::~Shader(void) {
	glDeleteProgram(_id);
}

void Shader::bind(void) {
	glUseProgram(_id);
}

}
