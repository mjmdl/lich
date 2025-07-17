#include "opengl_buffer.hpp"
#include "render.hpp"
#include "render_buffer.hpp"

namespace lich {

Buffer_Attrib::Buffer_Attrib(Shader_Data_Type type, const std::string &name) :
	name{name},
	type{type},
	offset{0} {}

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
	case Render_Api::Opengl:
		return std::make_unique<Opengl_Vertex_Array>();
		
	case Render_Api::None:
		return tl::unexpected{"Vertex_Array is not implemented for Render_Api::None."};
		
	default:
		return tl::unexpected{"Unknown Render_Api."};
	}
}

tl::expected<std::unique_ptr<Vertex_Buffer>, std::string> Vertex_Buffer::
create(const F32 *vertices, Usize count) {
	switch (Renderer_Api::api()) {
	case Render_Api::Opengl:
		return std::make_unique<Opengl_Vertex_Buffer>(vertices, count);
		
	case Render_Api::None:
		return tl::unexpected{"Vertex_Buffer is not implemented for Render_Api::None."};
		
	default:
		return tl::unexpected{"Unknown Render_Api."};
	}
}

tl::expected<std::unique_ptr<Index_Buffer>, std::string> Index_Buffer::
create(const U32 *indices, Usize count) {
	switch (Renderer_Api::api()) {
	case Render_Api::Opengl:
		return std::make_unique<Opengl_Index_Buffer>(indices, count);
		
	case Render_Api::None:
		return tl::unexpected{"Index_Buffer is not implemented for Render_Api::None."};
		
	default:
		return tl::unexpected{"Unknown Render_Api."};
	}
}

}
