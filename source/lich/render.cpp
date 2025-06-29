#include "opengl_render.hpp"

namespace lich {

tl::expected<std::unique_ptr<VertexArray>, std::string> VertexArray::create()
{
	switch (Renderer::api()) {
	case RenderApi::Opengl: return std::make_unique<OpenglVertexArray>();
	case RenderApi::None:   return tl::unexpected{"VertexArray is not implemented for RenderApi::None."};
	default:                return tl::unexpected{"Unknown RenderApi."};
	}
}

tl::expected<std::unique_ptr<VertexBuffer>, std::string>
VertexBuffer::create(const F32 *vertices, Usize count)
{
	switch (Renderer::api()) {
	case RenderApi::Opengl: return std::make_unique<OpenglVertexBuffer>(vertices, count);
	case RenderApi::None:   return tl::unexpected{"VertexBuffer is not implemented for RenderApi::None."};
	default:                return tl::unexpected{"Unknown RenderApi."};
	}
}

tl::expected<std::unique_ptr<IndexBuffer>, std::string>
IndexBuffer::create(const U32 *indices, Usize count)
{
	switch (Renderer::api()) {
	case RenderApi::Opengl: return std::make_unique<OpenglIndexBuffer>(indices, count);
	case RenderApi::None:   return tl::unexpected{"IndexBuffer is not implemented for RenderApi::None."};
	default:                return tl::unexpected{"Unknown RenderApi."};
	}
}

tl::expected<std::unique_ptr<Shader>, std::string>
Shader::create(const std::string &vertex_source, const std::string &fragment_source)
{
	switch (Renderer::api()) {
	case RenderApi::Opengl: return OpenglShader::compile(vertex_source, fragment_source);
	case RenderApi::None:   return tl::unexpected{"Shader is not implemented for RenderApi::None."};
	default:                return tl::unexpected{"Unknown RenderApi."};
	}
}

}
