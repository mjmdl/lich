#ifndef LICH_RENDER_HPP
#define LICH_RENDER_HPP

#include <glm/glm.hpp>
#include <tl/expected.hpp>

namespace lich {

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;

enum class RenderApi {
	None = 0,
	Opengl,
};

class RendererApi {
public:
	inline static RenderApi api() { return api_; }

	virtual void set_clear_color(const glm::vec4 &color) = 0;
	virtual void clear() = 0;
	virtual void draw_indexed(const std::unique_ptr<VertexArray> &vertex_array) = 0;

private:
	inline static RenderApi api_ = RenderApi::Opengl;
};

class RenderCommand {
public:
	inline static void set_clear_color(const glm::vec4 &color)
	{
		renderer_api_->set_clear_color(color);
	}
	
	inline static void clear()
	{
		renderer_api_->clear();
	}
		
	inline static void draw_indexed(const std::unique_ptr<VertexArray> &vertex_array)
	{
		renderer_api_->draw_indexed(vertex_array);
	}
	
private:
	static RendererApi *renderer_api_;
};

class Renderer {
public:
	static void begin_scene();
	static void end_scene();
	static void submit(const std::unique_ptr<VertexArray> &vertex_array);
};

class OrthographicCamera2d {
public:
	OrthographicCamera2d(float left, float right, float bottom, float top);

	const glm::vec3 &position() const { return _position; }
	float rotation() const { return _rotation; }
	const glm::mat4 &projection() const { return _projection; }
	const glm::mat4 &view() const { return _view; }
	const glm::mat4 &view_projection() const { return _view_projection; }
	void set_aspect_ratio(float aspect_ratio);

	void set_position(const glm::vec3 &position)
	{
		_position = position;
		_recalculate_view_matrix();
	}

	void set_rotation(float rotation)
	{
		_rotation = rotation;
		_recalculate_view_matrix();
	}
	
private:
	void _recalculate_view_matrix();
	
private:
	glm::mat4 _projection;
	glm::mat4 _view;
	glm::mat4 _view_projection;

	glm::vec3 _position;
	float _rotation;
	
	float _left;
	float _right;
	float _bottom;
	float _top;
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

class VertexArray {
public:
	static tl::expected<std::unique_ptr<VertexArray>, std::string> create();
	
	virtual ~VertexArray() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void add_vertex_buffer(std::unique_ptr<VertexBuffer> &&vbo) = 0;
	virtual void set_index_buffer(std::unique_ptr<IndexBuffer> &&ebo) = 0;
	virtual const std::unique_ptr<IndexBuffer> &index_buffer() const = 0;
	virtual Usize vertex_count() const = 0;
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
	virtual void upload_uniform(const std::string &name, const glm::mat4 &matrix) = 0;
};

}

#endif
