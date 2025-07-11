#ifndef SAND_RENDER_LAYER_HPP
#define SAND_RENDER_LAYER_HPP

#include <lich/layer.hpp>
#include <lich/render.hpp>

namespace sand {

class Render_Layer final : public lich::Layer {
public:
	Render_Layer(float aspect_ratio);
	void update() override;
	void handle(lich::Event &event) override;

private:
	std::unique_ptr<lich::Vertex_Array> _vertex_array;
	std::unique_ptr<lich::Shader> _shader;
	lich::Orthographic_Camera_2d _camera;
};

}

#endif
