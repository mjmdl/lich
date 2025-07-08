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

class OpenglRendererApi final: public RendererApi {
public:
	void set_clear_color(const glm::vec4 &color) override;
	void clear() override;
	void draw_indexed(const std::unique_ptr<VertexArray> &vertex_array) override;
};

class OpenglVertexArray: public VertexArray {
public:
	OpenglVertexArray();
	~OpenglVertexArray() override;
	void bind() override;
	void unbind() override;
	void add_vertex_buffer(std::unique_ptr<VertexBuffer> &&vbo) override;
	void set_index_buffer(std::unique_ptr<IndexBuffer> &&ebo) override;
	const std::unique_ptr<IndexBuffer> &index_buffer() const override { return _index_buffer; }
	Usize vertex_count() const { return _vertex_count; }

private:
	std::vector<std::unique_ptr<VertexBuffer>> _vertex_buffers;
	std::unique_ptr<IndexBuffer> _index_buffer;
	Usize _vertex_count;
	GLuint _vao;
};

class OpenglVertexBuffer final: public VertexBuffer {
public:
	OpenglVertexBuffer(const F32 *vertices, Usize count);
	~OpenglVertexBuffer() override;
	void bind() override;
	void unbind() override;
	void set_layout(const std::unique_ptr<Shader> &shader,
		const BufferLayout &layout) override;
	const BufferLayout &layout() const override { return _layout; }
	Usize size() const override { return _count * sizeof (F32); }

private:
	BufferLayout _layout;
	Usize _count;
	GLuint _vbo;
};

class OpenglIndexBuffer final: public IndexBuffer {
public:
	OpenglIndexBuffer(const U32 *indices, Usize count);
	~OpenglIndexBuffer() override;
	void bind() override;
	void unbind() override;
	Usize count() const override { return _count; }

private:
	GLuint _ebo;
	Usize _count;
};

class OpenglShader final: public Shader {
public:
	static tl::expected<std::unique_ptr<Shader>, std::string> compile(
		const std::string &vertex_source, const std::string &fragment_source);
	
	OpenglShader(GLuint program): _program{program} {}
	~OpenglShader() override { glDeleteProgram(_program); }
	void bind() override { glUseProgram(_program); }
	void unbind() override { glUseProgram(0); }
	void upload_uniform(const std::string &name, const glm::mat4 &matrix) override;
	
	void *handle() const override
	{
		return reinterpret_cast<void *>(static_cast<uintptr_t>(_program));
	}
	
private:
	GLuint _program;
};

}

#endif
