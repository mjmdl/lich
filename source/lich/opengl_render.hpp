#ifndef LICH_OPENGL_RENDER_HPP
#define LICH_OPENGL_RENDER_HPP

#include <glad/glad.h>

#include "render.hpp"

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

void pop_opengl_errors();
void check_opengl_error(const char *file, int line);
void assert_opengl_error(const char *file, int line);

class Opengl_Renderer_Api final : public Renderer_Api {
public:
	void set_clear_color(const glm::vec4 &color) override;
	void clear() override;
	void draw_indexed(const std::unique_ptr<Vertex_Array> &vertex_array) override;
};

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
	std::vector<std::unique_ptr<Vertex_Buffer>> _vertex_buffers;
	std::unique_ptr<Index_Buffer> _index_buffer;
	Usize _vertex_count;
	GLuint _vao;
};

class Opengl_Vertex_Buffer final : public Vertex_Buffer {
public:
	Opengl_Vertex_Buffer(const F32 *vertices, Usize count);
	~Opengl_Vertex_Buffer() override;
	void bind() override;
	void unbind() override;
	void set_layout(const std::unique_ptr<Shader> &shader, const Buffer_Layout &layout) override;
	const Buffer_Layout &layout() const override;
	Usize size() const override;

private:
	Buffer_Layout _layout;
	Usize _count;
	GLuint _vbo;
};

class Opengl_Index_Buffer final : public Index_Buffer {
public:
	Opengl_Index_Buffer(const U32 *indices, Usize count);
	~Opengl_Index_Buffer() override;
	void bind() override;
	void unbind() override;
	Usize count() const override;

private:
	GLuint _ebo;
	Usize _count;
};

class Opengl_Shader final : public Shader {
public:
	static tl::expected<std::unique_ptr<Shader>, std::string>
	compile(const std::string &vertex_source, const std::string &fragment_source);
	
	Opengl_Shader(GLuint program);
	~Opengl_Shader() override;
	void bind() override;
	void unbind() override;
	void upload_uniform(const std::string &name, const glm::mat4 &matrix) override;
	void *handle() const override;
	
private:
	GLuint _program;
};

}

#endif
