#include "render.hpp"
#include "render_shader.hpp"
#include "opengl_shader.hpp"

namespace lich {

tl::expected<std::unique_ptr<Shader>, std::string> Shader::
create(const std::string &vertex_source, const std::string &fragment_source) {
	switch (Renderer_Api::api()) {
	case Render_Api::Opengl:
		return Opengl_Shader::compile(vertex_source, fragment_source);
		
	case Render_Api::None:
		return tl::unexpected{"Shader is not implemented for Render_Api::None."};
		
	default:
		return tl::unexpected{"Unknown Render_Api."};
	}
}

}
