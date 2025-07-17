#include "opengl_render.hpp"

namespace lich {

Renderer_Api *Render_Command::renderer_api_ = new Opengl_Renderer_Api;

Render_Api Renderer_Api::api() {
	return api_;
}

void Render_Command::set_clear_color(const glm::vec4 &color) {
	renderer_api_->set_clear_color(color);
}
	
void Render_Command::clear() {
	renderer_api_->clear();
}
		
void Render_Command::draw_indexed(const std::unique_ptr<Vertex_Array> &vertex_array) {
	renderer_api_->draw_indexed(vertex_array);
}

void Renderer::begin_scene() {}
void Renderer::end_scene() {}

void Renderer::submit(const std::unique_ptr<Vertex_Array> &vertex_array) {
	vertex_array->bind();
	Render_Command::draw_indexed(vertex_array);
}

}
