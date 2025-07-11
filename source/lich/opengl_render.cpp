#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <tl/expected.hpp>

#include "log.hpp"
#include "opengl_render.hpp"

namespace lich {

void pop_opengl_errors() {
	GLenum error;
	do error = glGetError(); while (error != GL_NO_ERROR);
}

void check_opengl_error(const char *file, int line) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		log_error("OpenGL error at file {} line {}: #{}.", file, line, error);
	}
}

void assert_opengl_error(const char *file, int line) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		log_fatal("OpenGL error at file {} line {}: #{}.", file, line, error);
		LICH_ABORT();
	}
}

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

static GLenum equivalent_opengl_type(Shader_Data_Type type) {
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

/*
 * class Opengl_Renderer_Api
 */

void Opengl_Renderer_Api::set_clear_color(const glm::vec4 &color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Opengl_Renderer_Api::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Opengl_Renderer_Api::
draw_indexed(const std::unique_ptr<Vertex_Array> &vertex_array) {
	if (vertex_array->index_buffer()) {
		glDrawElements(GL_TRIANGLES, vertex_array->index_buffer()->count(),
			GL_UNSIGNED_INT, NULL);
	} else glDrawArrays(GL_TRIANGLES, 0, vertex_array->vertex_count());
}

/*
 * class Opengl_Vertex_Array
 */

Opengl_Vertex_Array::Opengl_Vertex_Array() : _vertex_count{0} {
	GL_CHECK(glCreateVertexArrays(1, &_vao));
	GL_CHECK(glBindVertexArray(_vao));
}

Opengl_Vertex_Array::~Opengl_Vertex_Array() {
	GL_CHECK(glDeleteVertexArrays(1, &_vao));
}

void Opengl_Vertex_Array::bind() { GL_CHECK(glBindVertexArray(_vao)); }
void Opengl_Vertex_Array::unbind() { GL_CHECK(glBindVertexArray(0)); }

void Opengl_Vertex_Array::add_vertex_buffer(std::unique_ptr<Vertex_Buffer> &&vbo) {
	GL_CHECK(glBindVertexArray(_vao));
	vbo->bind();

	Usize stride = 0;
	for (const auto &attrib : vbo->layout().attribs) {
		stride += size_of(attrib.type);
	}

	Usize offset = 0;
	Usize location = 0;
	for (const auto &attrib : vbo->layout().attribs) {
		GL_CHECK(glEnableVertexAttribArray(location));
		Usize components = component_count_of(attrib.type);
		GLenum type = equivalent_opengl_type(attrib.type);
		GLenum normalized = GL_FALSE;
		GL_CHECK(glVertexAttribPointer(
			location, components, type, normalized, stride, (void *)offset));
		offset += size_of(attrib.type);
		++location;
	}

	Usize buffer_size = vbo->size();
	Usize vertex_count = buffer_size / stride;
	if (_vertex_count != 0 and vertex_count != _vertex_count) {
		log_warn("Adding a vertex buffer of different number of elements: From {} to {}.",
			_vertex_count, vertex_count);
	}

	_vertex_count = vertex_count;
	_vertex_buffers.emplace_back(std::move(vbo));
}

void Opengl_Vertex_Array::set_index_buffer(std::unique_ptr<Index_Buffer> &&ebo) {
	GL_CHECK(glBindVertexArray(_vao));
	ebo->bind();

	_index_buffer = std::move(ebo);
}

const std::unique_ptr<Index_Buffer> &Opengl_Vertex_Array::index_buffer() const {
	return _index_buffer;
}

Usize Opengl_Vertex_Array::vertex_count() const { return _vertex_count; }

/*
 * class Opengl_Vertex_Buffer
 */

Opengl_Vertex_Buffer::Opengl_Vertex_Buffer(const F32 *vertices, Usize count) :
	_layout{}, _count{count}
{
	GL_CHECK(glCreateBuffers(1, &_vbo));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
	GL_CHECK(glBufferData(
		GL_ARRAY_BUFFER, count * (sizeof *vertices), vertices, GL_STATIC_DRAW));
}

Opengl_Vertex_Buffer::~Opengl_Vertex_Buffer() { GL_CHECK(glDeleteBuffers(1, &_vbo)); }

void Opengl_Vertex_Buffer::bind() { GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vbo)); }
void Opengl_Vertex_Buffer::unbind() { GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

void Opengl_Vertex_Buffer::
set_layout(const std::unique_ptr<Shader> &shader, const Buffer_Layout &layout) {
	_layout = layout;

	for (const auto &attrib : _layout.attribs) {
		const char *name = attrib.name.c_str();
		GLint location;
		GLuint program = static_cast<GLuint>(reinterpret_cast<uintptr_t>(shader->handle()));
		GL_CHECK(location = glGetAttribLocation(program, name));
		LICH_EXPECT(location >= 0, "Vertex location '{}' is not defined.", name);
	}
}

const Buffer_Layout &Opengl_Vertex_Buffer::layout() const { return _layout; }
Usize Opengl_Vertex_Buffer::size() const { return _count * sizeof (F32); }

/*
 * class OpenglIndex_Buffer
 */

Opengl_Index_Buffer::Opengl_Index_Buffer(const U32 *indices, Usize count) :
	_count{count}
{
	GL_CHECK(glCreateBuffers(1, &_ebo));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
	GL_CHECK(glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, count * (sizeof *indices), indices, GL_STATIC_DRAW));
}

Opengl_Index_Buffer::~Opengl_Index_Buffer() { GL_CHECK(glDeleteBuffers(1, &_ebo)); }

void Opengl_Index_Buffer::bind() { GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo)); }
void Opengl_Index_Buffer::unbind() { GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
Usize Opengl_Index_Buffer::count() const { return _count; }

/*
 * class Opengl_Shader
 */

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
		return tl::unexpected{fmt::v11::format(
			"Failed to compile a GLSL vertex shader: {}", vertex.error())};
	}

	source = reinterpret_cast<const GLchar *>(fragment_source.c_str());
	auto fragment = compile_shader_type_(source, GL_FRAGMENT_SHADER);
	if (!fragment) {
		glDeleteShader(vertex.value());
		return tl::unexpected{fmt::v11::format(
			"Failed to compile a GLSL fragment shader: {}", fragment.error())};
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

		return tl::unexpected{fmt::v11::format(
			"Failed to link a GLSL program: {}", log)};
	}

	return std::make_unique<Opengl_Shader>(program);
}

Opengl_Shader::Opengl_Shader(GLuint program) : _program{program} {}
Opengl_Shader::~Opengl_Shader() { glDeleteProgram(_program); }

void Opengl_Shader::bind() { glUseProgram(_program); }
void Opengl_Shader::unbind() { glUseProgram(0); }

void Opengl_Shader::
upload_uniform(const std::string &name, const glm::mat4 &matrix) {
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
