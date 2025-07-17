#ifndef LICH_OPENGL_RENDER_HPP
#define LICH_OPENGL_RENDER_HPP

#include "opengl.hpp"
#include "render.hpp"

namespace lich {

class Opengl_Renderer_Api final : public Renderer_Api {
public:
	void set_clear_color(const glm::vec4 &color) override;
	void clear() override;
	void draw_indexed(const std::unique_ptr<Vertex_Array> &vertex_array) override;
};

}

#endif
