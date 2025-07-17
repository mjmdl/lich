#ifndef LICH_OPENGL_BUFFER_HPP
#define LICH_OPENGL_BUFFER_HPP

#include "opengl.hpp"
#include "render_buffer.hpp"

namespace lich {

class Opengl_Vertex_Array : public Vertex_Array {
public:
	Opengl_Vertex_Array();
	~Opengl_Vertex_Array() override;
	void bind() override;
	void unbind() override;
	void add_vertex_buffer(std::unique_ptr<Vertex_Buffer> &&vbo) override;
	void set_index_buffer(std::unique_ptr<Index_Buffer> &&ebo) override;
	const std::unique_ptr<Index_Buffer> &index_buffer() const override;
	Usize vertex_count() const override;

private:
	std::vector<std::unique_ptr<Vertex_Buffer>> _vertex_buffers{};
	std::unique_ptr<Index_Buffer> _index_buffer{nullptr};
	Usize _vertex_count{0};
	GLuint _vao{0};
};

class Opengl_Vertex_Buffer final : public Vertex_Buffer {
public:
	Opengl_Vertex_Buffer(const F32 *vertices, Usize count);
	~Opengl_Vertex_Buffer() override;
	void bind() override;
	void unbind() override;
	void set_layout(
		const std::unique_ptr<Shader> &shader,
		const Buffer_Layout &layout
	) override;
	const Buffer_Layout &layout() const override;
	Usize size() const override;

private:
	Buffer_Layout _layout{};
	Usize _count{0};
	GLuint _vbo{0};
};

class Opengl_Index_Buffer final : public Index_Buffer {
public:
	Opengl_Index_Buffer(const U32 *indices, Usize count);
	~Opengl_Index_Buffer() override;
	void bind() override;
	void unbind() override;
	Usize count() const override;

private:
	GLuint _ebo{0};
	Usize _count{0};
};

}

#endif
