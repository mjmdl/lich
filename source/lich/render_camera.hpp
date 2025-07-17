#ifndef LICH_RENDER_CAMERA_HPP
#define LICH_RENDER_CAMERA_HPP

#include <glm/glm.hpp>
#include <tl/expected.hpp>

namespace lich {

class Orthographic_Camera_2d {
public:
	Orthographic_Camera_2d(float left, float right, float bottom, float top);

	const glm::vec3 &position() const;
	float rotation() const;
	const glm::mat4 &projection() const;
	const glm::mat4 &view() const;
	const glm::mat4 &view_projection() const;

	void set_aspect_ratio(float aspect_ratio);
	void set_position(const glm::vec3 &position);
	void set_rotation(float rotation);
	
private:
	void _recalculate_view_matrix();
	
private:
	glm::mat4 _projection{};
	glm::mat4 _view{};
	glm::mat4 _view_projection{};

	glm::vec3 _position{};
	float _rotation{0.0f};
	
	float _left{0.0f};
	float _right{0.0f};
	float _bottom{0.0f};
	float _top{0.0f};
};

}

#endif
