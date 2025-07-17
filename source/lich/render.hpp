#ifndef LICH_RENDER_HPP
#define LICH_RENDER_HPP

#include "render_buffer.hpp"

namespace lich {

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

}

#endif
