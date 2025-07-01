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
