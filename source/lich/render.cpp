#include <glm/gtc/matrix_transform.hpp>

#include "opengl_render.hpp"

namespace lich {

Renderer_Api *Render_Command::renderer_api_ = new Opengl_Renderer_Api;

Render_Api Renderer_Api::api() { return api_; }

void Render_Command::set_clear_color(const glm::vec4 &color) {
	renderer_api_->set_clear_color(color);
}
	
void Render_Command::clear() { renderer_api_->clear(); }
		
void Render_Command::draw_indexed(const std::unique_ptr<Vertex_Array> &vertex_array) {
	renderer_api_->draw_indexed(vertex_array);
}

void Renderer::begin_scene() {}
void Renderer::end_scene() {}

void Renderer::submit(const std::unique_ptr<Vertex_Array> &vertex_array) {
	vertex_array->bind();
	Render_Command::draw_indexed(vertex_array);
}

Orthographic_Camera_2d::Orthographic_Camera_2d(
	float left, float right, float bottom, float top) :
	_projection{glm::ortho(left, right, bottom, top, -1.0f, 1.0f)}, _view{1.0f},
	_position{0}, _rotation{0},
	_left{left}, _right{right}, _bottom{bottom}, _top{top}
{
	_view_projection = _projection * _view;
}

const glm::vec3 &Orthographic_Camera_2d::position() const { return _position; }
float Orthographic_Camera_2d::rotation() const { return _rotation; }
const glm::mat4 &Orthographic_Camera_2d::projection() const { return _projection; }
const glm::mat4 &Orthographic_Camera_2d::view() const { return _view; }
const glm::mat4 &Orthographic_Camera_2d::view_projection() const { return _view_projection; }

void Orthographic_Camera_2d::set_aspect_ratio(float aspect_ratio) {
	float center_x = (_left + _right) / 2.0f;
	float height = _top - _bottom;
	float width = height * aspect_ratio;

	_left = center_x - width / 2.0f;
	_right = center_x + width / 2.0f;

	_projection = glm::ortho(_left, _right, _bottom, _top, -1.0f, 1.0f);
	_view_projection = _projection * _view;
}

void Orthographic_Camera_2d::set_position(const glm::vec3 &position) {
	_position = position;
	_recalculate_view_matrix();
}

void Orthographic_Camera_2d::set_rotation(float rotation) {
	_rotation = rotation;
	_recalculate_view_matrix();
}

void Orthographic_Camera_2d::_recalculate_view_matrix() {
	const glm::mat4 identity{1.0f};
	const glm::vec3 up{0.0f, 0.0f, 1.0f};
	glm::mat4 transform = glm::translate(identity, _position) *
		glm::rotate(identity, _rotation, up);
	
	_view = glm::inverse(transform);
	_view_projection = _projection * _view;
}

Buffer_Attrib::Buffer_Attrib(Shader_Data_Type type, const std::string &name) :
	name{name}, type{type}, offset{0} {}

Buffer_Layout::Buffer_Layout(const std::initializer_list<Buffer_Attrib> &attribs) :
	attribs{attribs}
{
	calculate();
}

void Buffer_Layout::calculate() {
	Usize offset = 0;
	for (auto &attrib : attribs) {
		attrib.offset = offset;
		offset += size_of(attrib.type);
	}
	stride = offset;
}

tl::expected<std::unique_ptr<Vertex_Array>, std::string> Vertex_Array::create() {
	switch (Renderer_Api::api()) {
	case Render_Api::Opengl: return std::make_unique<Opengl_Vertex_Array>();
	case Render_Api::None:   return tl::unexpected{"Vertex_Array is not implemented for Render_Api::None."};
	default:                 return tl::unexpected{"Unknown Render_Api."};
	}
}

tl::expected<std::unique_ptr<Vertex_Buffer>, std::string> Vertex_Buffer::
create(const F32 *vertices, Usize count) {
	switch (Renderer_Api::api()) {
	case Render_Api::Opengl: return std::make_unique<Opengl_Vertex_Buffer>(vertices, count);
	case Render_Api::None:   return tl::unexpected{"Vertex_Buffer is not implemented for Render_Api::None."};
	default:                 return tl::unexpected{"Unknown Render_Api."};
	}
}

tl::expected<std::unique_ptr<Index_Buffer>, std::string> Index_Buffer::
create(const U32 *indices, Usize count) {
	switch (Renderer_Api::api()) {
	case Render_Api::Opengl: return std::make_unique<Opengl_Index_Buffer>(indices, count);
	case Render_Api::None:   return tl::unexpected{"Index_Buffer is not implemented for Render_Api::None."};
	default:                 return tl::unexpected{"Unknown Render_Api."};
	}
}

tl::expected<std::unique_ptr<Shader>, std::string> Shader::
create(const std::string &vertex_source, const std::string &fragment_source) {
	switch (Renderer_Api::api()) {
	case Render_Api::Opengl: return Opengl_Shader::compile(vertex_source, fragment_source);
	case Render_Api::None:   return tl::unexpected{"Shader is not implemented for Render_Api::None."};
	default:                 return tl::unexpected{"Unknown Render_Api."};
	}
}

}
