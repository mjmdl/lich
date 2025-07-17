#include <glm/gtc/type_ptr.hpp>

#include "log.hpp"
#include "opengl.hpp"
#include "opengl_shader.hpp"

namespace lich {

Usize component_count_of(Shader_Data_Type type) {
	switch (type) {
	case Shader_Data_Type::None:   return 0;
	case Shader_Data_Type::Bool:   return 1;
	case Shader_Data_Type::Int:    return 1;
	case Shader_Data_Type::Int2:   return 2;
	case Shader_Data_Type::Int3:   return 3;
	case Shader_Data_Type::Int4:   return 4;
	case Shader_Data_Type::Float:  return 1;
	case Shader_Data_Type::Float2: return 2;
	case Shader_Data_Type::Float3: return 3;
	case Shader_Data_Type::Float4: return 4;
	case Shader_Data_Type::Mat3:   return 3 * 3;
	case Shader_Data_Type::Mat4:   return 4 * 4;
	default:                       LICH_UNREACHABLE();
	}
}

Usize size_of(Shader_Data_Type type) {
	Usize n = component_count_of(type);
	switch (type) {
	case Shader_Data_Type::None:   return 0;
	case Shader_Data_Type::Bool:   return n * sizeof (GLboolean);
	case Shader_Data_Type::Int:    return n * sizeof (GLint);
	case Shader_Data_Type::Int2:   return n * sizeof (GLint);
	case Shader_Data_Type::Int3:   return n * sizeof (GLint);
	case Shader_Data_Type::Int4:   return n * sizeof (GLint);
	case Shader_Data_Type::Float:  return n * sizeof (GLfloat);
	case Shader_Data_Type::Float2: return n * sizeof (GLfloat);
	case Shader_Data_Type::Float3: return n * sizeof (GLfloat);
	case Shader_Data_Type::Float4: return n * sizeof (GLfloat);
	case Shader_Data_Type::Mat3:   return n * sizeof (GLfloat);
	case Shader_Data_Type::Mat4:   return n * sizeof (GLfloat);
	default:                       LICH_UNREACHABLE();
	}
}

GLenum equivalent_opengl_type(Shader_Data_Type type) {
	switch (type) {
	case Shader_Data_Type::None:   LICH_UNREACHABLE();
	case Shader_Data_Type::Bool:   return GL_BOOL;
	case Shader_Data_Type::Int:    return GL_INT;
	case Shader_Data_Type::Int2:   return GL_INT;
	case Shader_Data_Type::Int3:   return GL_INT;
	case Shader_Data_Type::Int4:   return GL_INT;
	case Shader_Data_Type::Float:  return GL_FLOAT;
	case Shader_Data_Type::Float2: return GL_FLOAT;
	case Shader_Data_Type::Float3: return GL_FLOAT;
	case Shader_Data_Type::Float4: return GL_FLOAT;
	case Shader_Data_Type::Mat3:   return GL_FLOAT;
	case Shader_Data_Type::Mat4:   return GL_FLOAT;
	default:                       LICH_UNREACHABLE();
	}
}


static tl::expected<GLuint, std::string>
compile_shader_type_(const GLchar *source, GLenum type) {
	GLuint shader;
	GL_CHECK(shader = glCreateShader(type));
	GL_CHECK(glShaderSource(shader, 1, &source, NULL));
	GL_CHECK(glCompileShader(shader));

	GLint status = GL_FALSE;
	GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
	if (status == GL_FALSE) {
		GLint length = 0;
		GL_CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));

		std::string log(static_cast<Usize>(length), '\0');
		GL_CHECK(glGetShaderInfoLog(shader, length, &length, log.data()));
		GL_CHECK(glDeleteShader(shader));

		return tl::unexpected{log};
	}

	return shader;
}

tl::expected<std::unique_ptr<Shader>, std::string> Opengl_Shader::
compile(const std::string &vertex_source, const std::string &fragment_source) {
	const GLchar *source = reinterpret_cast<const GLchar *>(vertex_source.c_str());
	auto vertex = compile_shader_type_(source, GL_VERTEX_SHADER);
	if (!vertex) {
		return tl::unexpected{
			fmt::v11::format(
				"Failed to compile a GLSL vertex shader: {}",
				vertex.error()
			)
		};
	}

	source = reinterpret_cast<const GLchar *>(fragment_source.c_str());
	auto fragment = compile_shader_type_(source, GL_FRAGMENT_SHADER);
	if (!fragment) {
		glDeleteShader(vertex.value());
		return tl::unexpected{
			fmt::v11::format(
				"Failed to compile a GLSL fragment shader: {}",
				fragment.error()
			)
		};
	}
	
	GLuint program = glCreateProgram();
	GL_CHECK(glAttachShader(program, vertex.value()));
	GL_CHECK(glAttachShader(program, fragment.value()));
	GL_CHECK(glLinkProgram(program));

	GL_CHECK(glDeleteShader(vertex.value()));
	GL_CHECK(glDeleteShader(fragment.value()));

	GLint status = GL_FALSE;
	GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &status));
	if (status == GL_FALSE) {
		GLint length = 0;
		GL_CHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

		std::string log(static_cast<Usize>(length), '\0');
		GL_CHECK(glGetProgramInfoLog(program, length, &length, log.data()));
		GL_CHECK(glDeleteProgram(program));

		return tl::unexpected{
			fmt::v11::format("Failed to link a GLSL program: {}", log)
		};
	}

	return std::make_unique<Opengl_Shader>(program);
}

Opengl_Shader::Opengl_Shader(GLuint program) :
	_program{program} {}

Opengl_Shader::~Opengl_Shader() {
	glDeleteProgram(_program);
}

void Opengl_Shader::bind() {
	glUseProgram(_program);
}

void Opengl_Shader::unbind() {
	glUseProgram(0);
}

void Opengl_Shader::upload_uniform(
	const std::string &name,
	const glm::mat4 &matrix
) {
	GL_CHECK(glUseProgram(_program));

	GLint location;
	const char *cstring = name.c_str();
	GL_CHECK(location = glGetUniformLocation(_program, cstring));
	if (location < 0) log_warn("GLSL uniform location '{}' not found.", name);
	
	GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void *Opengl_Shader::handle() const {
	return reinterpret_cast<void *>(static_cast<uintptr_t>(_program));
}

}
