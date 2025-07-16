#ifndef LICH_RENDER_HPP
#define LICH_RENDER_HPP

#include <glm/glm.hpp>
#include <tl/expected.hpp>

namespace lich {

class Vertex_Array;
class Vertex_Buffer;
class Index_Buffer;
class Shader;

enum class Render_Api {
	None = 0,
	Opengl,
};

class Renderer_Api {
public:
	static Render_Api api();

	virtual void set_clear_color(const glm::vec4 &color) = 0;
	virtual void clear() = 0;
	virtual void draw_indexed(const std::unique_ptr<Vertex_Array> &vertex_array) = 0;

private:
	inline static Render_Api api_ = Render_Api::Opengl;
};

class Render_Command {
public:
	static void set_clear_color(const glm::vec4 &color);
	static void clear();
	static void draw_indexed(const std::unique_ptr<Vertex_Array> &vertex_array);
	
private:
	static Renderer_Api *renderer_api_;
};

class Renderer {
public:
	static void begin_scene();
	static void end_scene();
	static void submit(const std::unique_ptr<Vertex_Array> &vertex_array);
};

class Orthographic_Camera_2d {
public:
	Orthographic_Camera_2d(float left, float right, float bottom, float top);

	const glm::vec3 &position() const;
	float rotation() const;
	const glm::mat4 &projection() const;
	const glm::mat4 &view() const;
	const glm::mat4 &view_projection() const;

	void set_aspect_ratio(float aspect_ratio);
	void set_position(const glm::vec3 &position);
	void set_rotation(float rotation);
	
private:
	void _recalculate_view_matrix();
	
private:
	glm::mat4 _projection{};
	glm::mat4 _view{};
	glm::mat4 _view_projection{};

	glm::vec3 _position{};
	float _rotation{0.0f};
	
	float _left{0.0f};
	float _right{0.0f};
	float _bottom{0.0f};
	float _top{0.0f};
};

enum class Shader_Data_Type {
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

Usize component_count_of(Shader_Data_Type type);
Usize size_of(Shader_Data_Type type);

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

class Vertex_Buffer {
public:
	static tl::expected<std::unique_ptr<Vertex_Buffer>, std::string>
	create(const F32 *vertices, Usize count);

	virtual ~Vertex_Buffer() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void set_layout(const std::unique_ptr<Shader> &shader,
		const Buffer_Layout &layout) = 0;
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

class Shader {
public:
	static tl::expected<std::unique_ptr<Shader>, std::string>
	create(const std::string &vertex_source, const std::string &fragment_source);

	virtual ~Shader() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void *handle() const = 0;
	virtual void
	upload_uniform(const std::string &name, const glm::mat4 &matrix) = 0;
};

}

#endif
