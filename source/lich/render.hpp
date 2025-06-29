#ifndef LICH_RENDER_HPP
#define LICH_RENDER_HPP

#include <tl/expected.hpp>

namespace lich {

enum class RenderApi {
	None = 0,
	Opengl,
};

class Renderer {
public:
	static RenderApi api() { return RenderApi::Opengl; }
};

enum class ShaderDataType {
	None = 0,
	Bool,
	Int,
	Int2,
	Int3,
	Int4,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
};

Usize component_count_of(ShaderDataType type);
Usize size_of(ShaderDataType type);

struct BufferAttrib {
	std::string name;
	ShaderDataType type;
	Usize offset;

	BufferAttrib(ShaderDataType type, const std::string &name):
		name{name}, type{type}, offset{0} {}
};

struct BufferLayout {
	std::vector<BufferAttrib> attribs;
	Usize stride;

	BufferLayout(const std::initializer_list<BufferAttrib> &attribs): attribs{attribs}
	{
		calculate();
	}

	void calculate()
	{
		Usize offset = 0;
		for (auto &attrib : attribs) {
			attrib.offset = offset;
			offset += size_of(attrib.type);
		}
		stride = offset;
	}
};

class VertexBuffer;
class IndexBuffer;
class Shader;

class VertexArray {
public:
	static tl::expected<std::unique_ptr<VertexArray>, std::string> create();
	
	virtual ~VertexArray() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void add_vertex_buffer(std::unique_ptr<VertexBuffer> &&vbo) = 0;
	virtual void set_index_buffer(std::unique_ptr<IndexBuffer> &&ebo) = 0;
	virtual void draw() = 0;
};

class VertexBuffer {
public:
	static tl::expected<std::unique_ptr<VertexBuffer>, std::string> create(
		const F32 *vertices, Usize count);

	virtual ~VertexBuffer() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void set_layout(const std::unique_ptr<Shader> &shader,
		const BufferLayout &layout) = 0;
	virtual const BufferLayout &layout() const = 0;
	virtual Usize size() const = 0;
};

class IndexBuffer {
public:
	static tl::expected<std::unique_ptr<IndexBuffer>, std::string> create(
		const U32 *indices, Usize count);	

	virtual ~IndexBuffer() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual Usize count() const = 0;
};

class Shader {
public:
	static tl::expected<std::unique_ptr<Shader>, std::string> create(
		const std::string &vertex_source, const std::string &fragment_source);

	virtual ~Shader() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void *handle() const = 0;
};

}

#endif
