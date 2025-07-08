#include <glm/gtc/matrix_transform.hpp>

#include "opengl_render.hpp"

namespace lich {

RendererApi *RenderCommand::renderer_api_ = new OpenglRendererApi;

void Renderer::begin_scene()
{
}

void Renderer::end_scene()
{
}

void Renderer::submit(const std::unique_ptr<VertexArray> &vertex_array)
{
	vertex_array->bind();
	RenderCommand::draw_indexed(vertex_array);
}

OrthographicCamera2d::OrthographicCamera2d(
	float left, float right, float bottom, float top):
	_projection{glm::ortho(left, right, bottom, top, -1.0f, 1.0f)}, _view{1.0f},
	_left{left}, _right{right}, _bottom{bottom}, _top{top}
{
	_view_projection = _projection * _view;
}

void OrthographicCamera2d::set_aspect_ratio(float aspect_ratio)
{
	float center_x = (_left + _right) / 2.0f;
	float height = _top - _bottom;
	float width = height * aspect_ratio;

	_left = center_x - width / 2.0f;
	_right = center_x + width / 2.0f;

	_projection = glm::ortho(_left, _right, _bottom, _top, -1.0f, 1.0f);
	_view_projection = _projection * _view;
}

void OrthographicCamera2d::_recalculate_view_matrix()
{
	const glm::mat4 identity{1.0f};
	const glm::vec3 up{0.0f, 0.0f, 1.0f};
	glm::mat4 transform = glm::translate(identity, _position) *
		glm::rotate(identity, _rotation, up);
	
	_view = glm::inverse(transform);
	_view_projection = _projection * _view;
}

tl::expected<std::unique_ptr<VertexArray>, std::string> VertexArray::create()
{
	switch (RendererApi::api()) {
	case RenderApi::Opengl: return std::make_unique<OpenglVertexArray>();
	case RenderApi::None:   return tl::unexpected{"VertexArray is not implemented for RenderApi::None."};
	default:                return tl::unexpected{"Unknown RenderApi."};
	}
}

tl::expected<std::unique_ptr<VertexBuffer>, std::string>
VertexBuffer::create(const F32 *vertices, Usize count)
{
	switch (RendererApi::api()) {
	case RenderApi::Opengl: return std::make_unique<OpenglVertexBuffer>(vertices, count);
	case RenderApi::None:   return tl::unexpected{"VertexBuffer is not implemented for RenderApi::None."};
	default:                return tl::unexpected{"Unknown RenderApi."};
	}
}

tl::expected<std::unique_ptr<IndexBuffer>, std::string>
IndexBuffer::create(const U32 *indices, Usize count)
{
	switch (RendererApi::api()) {
	case RenderApi::Opengl: return std::make_unique<OpenglIndexBuffer>(indices, count);
	case RenderApi::None:   return tl::unexpected{"IndexBuffer is not implemented for RenderApi::None."};
	default:                return tl::unexpected{"Unknown RenderApi."};
	}
}

tl::expected<std::unique_ptr<Shader>, std::string>
Shader::create(const std::string &vertex_source, const std::string &fragment_source)
{
	switch (RendererApi::api()) {
	case RenderApi::Opengl: return OpenglShader::compile(vertex_source, fragment_source);
	case RenderApi::None:   return tl::unexpected{"Shader is not implemented for RenderApi::None."};
	default:                return tl::unexpected{"Unknown RenderApi."};
	}
}

}
