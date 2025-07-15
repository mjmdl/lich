#ifndef SAND_RENDER_LAYER_HPP
#define SAND_RENDER_LAYER_HPP

#include <lich/layer.hpp>
#include <lich/render.hpp>

namespace sand {

class Render_Layer final : public lich::Layer {
private:
	enum Key_ { W = 0, A = 1, S = 2, D = 3, Q = 4, E = 5, Count };

public:
	Render_Layer(float aspect_ratio);
	void update(lich::Timestep timestep) override;
	void handle(lich::Event &event) override;

private:
	std::unique_ptr<lich::Vertex_Array> _vertex_array;
	std::unique_ptr<lich::Shader> _shader;
	lich::Orthographic_Camera_2d _camera;
	bool _keys[Count];
};

}

#endif
