#include "render_layer.hpp"

namespace sand {

using namespace lich::types;

static const F32 vertices_[] = {
	-0.5f, -0.5f, 0.2f, 0.4f, 0.8f,
	-0.5f,  0.5f, 0.8f, 0.2f, 0.4f,
	0.5f,  0.5f, 0.2f, 0.8f, 0.2f,
	0.5f, -0.5f, 0.8f, 0.4f, 1.2f};
static Usize vertex_count_ = (sizeof vertices_) / (sizeof vertices_[0]);

static const U32 indices_[] = {
	0, 1, 2,
	0, 2, 3};
static Usize index_count_ = (sizeof indices_) / (sizeof indices_[0]);

const char *vertex_source_ = R"glsl(
	#version 330 core
	
	layout(location = 0) in vec2 pos;
	layout(location = 1) in vec3 color;
	out vec3 v_color;
	
	void main()
	{
		gl_Position = vec4(pos.xy, 0, 1);
		v_color     = color;
	})glsl";
const char *fragment_source_ = R"glsl(
	#version 330 core
	
	in  vec3 v_color;
	out vec4 f_color;
	
	void main()
	{
		f_color = vec4(v_color.xyz, 1);
	})glsl";

RenderLayer::RenderLayer()
{
	using namespace lich;

	auto vao_result = VertexArray::create();
	if (!vao_result) {
		log_fatal("{}", vao_result.error());
		LICH_ABORT();
	}
	_vertex_array = std::move(vao_result.value());

	auto shader_result = Shader::create(vertex_source_, fragment_source_);
	if (!shader_result) {
		log_fatal("{}", shader_result.error());
		LICH_ABORT();
	}
	_shader = std::move(shader_result.value());
	_shader->bind();
	
	auto vbo_result = VertexBuffer::create(vertices_, vertex_count_);
	if (!vbo_result) {
		log_fatal("{}", vbo_result.error());
		LICH_ABORT();
	}
	auto vertex_buffer = std::move(vbo_result.value());
	log_debug("Foo");
	vertex_buffer->set_layout(_shader, BufferLayout{
		{ShaderDataType::Float2, "pos"},
		{ShaderDataType::Float3, "color"}});
	log_debug("Bar");
	_vertex_array->add_vertex_buffer(std::move(vertex_buffer));
	
	auto ebo_result = IndexBuffer::create(indices_, index_count_);
	if (!ebo_result) {
		log_fatal("{}", ebo_result.error());
		LICH_ABORT();
	}
	_vertex_array->set_index_buffer(std::move(ebo_result.value()));
}

void RenderLayer::update()
{
	_shader->bind();
	lich::Renderer::submit(_vertex_array);
}

}
