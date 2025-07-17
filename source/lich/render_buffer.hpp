#ifndef LICH_RENDER_BUFFER_HPP
#define LICH_RENDER_BUFFER_HPP

#include <tl/expected.hpp>

#include "render_shader.hpp"

namespace lich {

struct Buffer_Attrib {
	std::string name{};
	Shader_Data_Type type{Shader_Data_Type::None};
	Usize offset{0};

	Buffer_Attrib(Shader_Data_Type type, const std::string &name);
};

struct Buffer_Layout {
	std::vector<Buffer_Attrib> attribs{};
	Usize stride{0};

	Buffer_Layout(const std::initializer_list<Buffer_Attrib> &attribs);
	void calculate();
};

class Vertex_Buffer {
public:
	static tl::expected<std::unique_ptr<Vertex_Buffer>, std::string>
	create(const F32 *vertices, Usize count);

	virtual ~Vertex_Buffer() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void set_layout(
		const std::unique_ptr<Shader> &shader,
		const Buffer_Layout &layout
	) = 0;
	virtual const Buffer_Layout &layout() const = 0;
	virtual Usize size() const = 0;
};

class Index_Buffer {
public:
	static tl::expected<std::unique_ptr<Index_Buffer>, std::string>
	create(const U32 *indices, Usize count);	

	virtual ~Index_Buffer() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual Usize count() const = 0;
};

class Vertex_Array {
public:
	static tl::expected<std::unique_ptr<Vertex_Array>, std::string> create();
	
	virtual ~Vertex_Array() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void add_vertex_buffer(std::unique_ptr<Vertex_Buffer> &&vbo) = 0;
	virtual void set_index_buffer(std::unique_ptr<Index_Buffer> &&ebo) = 0;
	virtual const std::unique_ptr<Index_Buffer> &index_buffer() const = 0;
	virtual Usize vertex_count() const = 0;
};

}

#endif
