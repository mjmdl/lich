#include "log.hpp"
#include "opengl.hpp"
#include "opengl_buffer.hpp"

namespace lich {

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

void Opengl_Vertex_Array::bind() {
	GL_CHECK(glBindVertexArray(_vao));
}

void Opengl_Vertex_Array::unbind() {
	GL_CHECK(glBindVertexArray(0));
}

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
			location,
			components,
			type,
			normalized,
			stride,
			(void *)offset
		));
		offset += size_of(attrib.type);
		++location;
	}

	Usize buffer_size = vbo->size();
	Usize vertex_count = buffer_size / stride;
	if (_vertex_count != 0 and vertex_count != _vertex_count) {
		log_warn(
			"Adding a vertex buffer of different number of elements: From {} to {}.",
			_vertex_count,
			vertex_count
		);
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

Usize Opengl_Vertex_Array::vertex_count() const {
	return _vertex_count;
}

/*
 * class Opengl_Vertex_Buffer
 */

Opengl_Vertex_Buffer::Opengl_Vertex_Buffer(const F32 *vertices, Usize count) :
	_layout{},
	_count{count}
{
	GL_CHECK(glCreateBuffers(1, &_vbo));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
	GL_CHECK(glBufferData(
		GL_ARRAY_BUFFER,
		count * (sizeof *vertices),
		vertices,
		GL_STATIC_DRAW
	));
}

Opengl_Vertex_Buffer::~Opengl_Vertex_Buffer() {
	GL_CHECK(glDeleteBuffers(1, &_vbo));
}

void Opengl_Vertex_Buffer::bind() {
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
}

void Opengl_Vertex_Buffer::unbind() {
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Opengl_Vertex_Buffer::
set_layout(const std::unique_ptr<Shader> &shader, const Buffer_Layout &layout) {
	_layout = layout;

	for (const auto &attrib : _layout.attribs) {
		const char *name = attrib.name.c_str();
		GLint location;
		GLuint program =
			static_cast<GLuint>(reinterpret_cast<uintptr_t>(shader->handle()));
		GL_CHECK(location = glGetAttribLocation(program, name));
		LICH_EXPECT(location >= 0, "Vertex location '{}' is not defined.", name);
	}
}

const Buffer_Layout &Opengl_Vertex_Buffer::layout() const {
	return _layout;
}

Usize Opengl_Vertex_Buffer::size() const {
	return _count * sizeof (F32);
}

/*
 * class OpenglIndex_Buffer
 */

Opengl_Index_Buffer::Opengl_Index_Buffer(const U32 *indices, Usize count) :
	_count{count}
{
	GL_CHECK(glCreateBuffers(1, &_ebo));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
	GL_CHECK(glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		count * (sizeof *indices),
		indices,
		GL_STATIC_DRAW
	));
}

Opengl_Index_Buffer::~Opengl_Index_Buffer() {
	GL_CHECK(glDeleteBuffers(1, &_ebo));
}

void Opengl_Index_Buffer::bind() {
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
}

void Opengl_Index_Buffer::unbind() {
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

Usize Opengl_Index_Buffer::count() const {
	return _count;
}

}
