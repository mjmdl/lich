#ifndef LICH_RENDER_HPP
#define LICH_RENDER_HPP

#include "render_buffer.hpp"
#include "render_camera.hpp"

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

struct Scene_Data {
	glm::mat4 view_projection{0.0f};
};

class Renderer {
public:
	static void begin_scene();
	static void end_scene();
	static void submit(const lich::Orthographic_Camera_2d &camera);
	static void submit(
		const std::unique_ptr<Shader> &shader,
		const std::unique_ptr<Vertex_Array> &vertex_array,
		const glm::mat4 &transform = glm::mat4{1.0f}
	);

private:
	inline static Scene_Data scene_data_{};
};

}

#endif
