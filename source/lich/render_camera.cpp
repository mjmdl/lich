#include <glm/gtc/matrix_transform.hpp>

#include "render_camera.hpp"

namespace lich {

Orthographic_Camera_2d::Orthographic_Camera_2d(
	float left,
	float right,
	float bottom,
	float top
) :
	_projection{glm::ortho(left, right, bottom, top, -1.0f, 1.0f)},
	_view{1.0f},
	_position{0},
	_rotation{0},
	_left{left},
	_right{right},
	_bottom{bottom},
	_top{top}
{
	_view_projection = _projection * _view;
}

const glm::vec3 &Orthographic_Camera_2d::position() const {
	return _position;
}

float Orthographic_Camera_2d::rotation() const {
	return _rotation;
}

const glm::mat4 &Orthographic_Camera_2d::projection() const {
	return _projection;
}

const glm::mat4 &Orthographic_Camera_2d::view() const {
	return _view;
}

const glm::mat4 &Orthographic_Camera_2d::view_projection() const {
	return _view_projection;
}

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
	glm::mat4 transform =
		glm::translate(identity, _position) *
		glm::rotate(identity, _rotation, up);
	
	_view = glm::inverse(transform);
	_view_projection = _projection * _view;
}

}
