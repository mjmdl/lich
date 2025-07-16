#include "render_layer.hpp"

namespace sand {

using namespace lich::types;

static const F32 vertices_[] = {
	-0.5f, -0.5f, 0.2f, 0.4f, 0.8f,
	-0.5f,  0.5f, 0.8f, 0.2f, 0.4f,
	0.5f,  0.5f, 0.2f, 0.8f, 0.2f,
	0.5f, -0.5f, 0.8f, 0.4f, 1.2f
};
static Usize vertex_count_ = (sizeof vertices_) / (sizeof vertices_[0]);

static const U32 indices_[] = {
	0, 1, 2,
	0, 2, 3
};
static Usize index_count_ = (sizeof indices_) / (sizeof indices_[0]);

const char *vertex_source_ = R"glsl(
	#version 330 core
	
	layout(location = 0) in vec2 pos;
	layout(location = 1) in vec3 color;
	out vec3 v_color;

	uniform mat4 u_view_projection;

	void main() {
		gl_Position = u_view_projection * vec4(pos.xy, 0, 1);
		v_color     = color;
	}
)glsl";
const char *fragment_source_ = R"glsl(
	#version 330 core
	
	in  vec3 v_color;
	out vec4 f_color;
	
	void main() {
		f_color = vec4(v_color.xyz, 1);
	}
)glsl";

Render_Layer::Render_Layer(float aspect_ratio) :
	_camera{-1.0f, 1.0f, -1.0f, 1.0f},
	_keys{}
{
	using namespace lich;

	_camera.set_aspect_ratio(aspect_ratio);

	auto vao_result = Vertex_Array::create();
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
	
	auto vbo_result = Vertex_Buffer::create(vertices_, vertex_count_);
	if (!vbo_result) {
		log_fatal("{}", vbo_result.error());
		LICH_ABORT();
	}
	auto vertex_buffer = std::move(vbo_result.value());
	log_debug("Foo");
	vertex_buffer->set_layout(
		_shader,
		Buffer_Layout{
			{Shader_Data_Type::Float2, "pos"},
			{Shader_Data_Type::Float3, "color"}
		}
	);
	log_debug("Bar");
	_vertex_array->add_vertex_buffer(std::move(vertex_buffer));
	
	auto ebo_result = Index_Buffer::create(indices_, index_count_);
	if (!ebo_result) {
		log_fatal("{}", ebo_result.error());
		LICH_ABORT();
	}
	_vertex_array->set_index_buffer(std::move(ebo_result.value()));
}

void Render_Layer::update([[maybe_unused]] lich::Timestep timestep) {
	glm::vec3 direction{};
	if (_keys[W]) direction.y += 1.0f;
	if (_keys[A]) direction.x -= 1.0f;
	if (_keys[S]) direction.y -= 1.0f;
	if (_keys[D]) direction.x += 1.0f;
	
	if (direction != glm::vec3{0}) {
		direction = glm::normalize(direction);
	
		float angle = _camera.rotation();
		glm::mat2 rotation_matrix{
			{ std::cos(angle), std::sin(angle)},
			{-std::sin(angle), std::cos(angle)}
		};
		glm::vec2 rotated = rotation_matrix * glm::vec2{direction.x, direction.y};
		direction = {rotated.x, rotated.y, 0.0f};
	
		float camera_speed = 1.0f * timestep.seconds();
		auto new_pos = _camera.position() + direction * camera_speed;
		_camera.set_position(new_pos);
	}

	float rotation = 0;
	if (_keys[Q]) rotation -= -1;
	if (_keys[E]) rotation += -1;

	if (rotation != 0) {
		float degrees_per_frame = 1.0f * timestep.seconds();
		float angle = _camera.rotation() + rotation * degrees_per_frame;
		_camera.set_rotation(angle);
	}
	
	_shader->bind();
	_shader->upload_uniform("u_view_projection", _camera.view_projection());
	lich::Renderer::submit(_vertex_array);
}

void Render_Layer::handle(lich::Event &event) {
	lich::Event_Dispatcher dispatcher{event};

	dispatcher.handle<lich::Window_Size_Event>(
		[this] (const auto &size) -> bool {
			float aspect_ratio = (float)size.width / (float)size.height;
			_camera.set_aspect_ratio(aspect_ratio);
			return false;
		}
	);

	dispatcher.handle<lich::Key_Press_Event>(
		[this] (const auto &press) -> bool {
			using namespace lich;
			if (press.repeat != 0) return false;
			if (press.code == Key_Code::W) _keys[W] = true;
			if (press.code == Key_Code::A) _keys[A] = true;
			if (press.code == Key_Code::S) _keys[S] = true;
			if (press.code == Key_Code::D) _keys[D] = true;
			if (press.code == Key_Code::E) _keys[E] = true;
			if (press.code == Key_Code::Q) _keys[Q] = true;
			return false;
		}
	);

	dispatcher.handle<lich::Key_Release_Event>(
		[this] (const auto &release) -> bool {
			using namespace lich;
			if (release.code == Key_Code::W) _keys[W] = false;
			if (release.code == Key_Code::A) _keys[A] = false;
			if (release.code == Key_Code::S) _keys[S] = false;
			if (release.code == Key_Code::D) _keys[D] = false;
			if (release.code == Key_Code::E) _keys[E] = false;
			if (release.code == Key_Code::Q) _keys[Q] = false;
			return false;
		}
	);
}

}
