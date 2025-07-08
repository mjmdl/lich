#ifndef SAND_RENDER_LAYER_HPP
#define SAND_RENDER_LAYER_HPP

#include <lich/layer.hpp>
#include <lich/render.hpp>

namespace sand {

class RenderLayer final: public lich::Layer {
public:
	RenderLayer(float aspect_ratio);
	void update() override;
	void handle(lich::Event &event) override;

private:
	std::unique_ptr<lich::VertexArray> _vertex_array;
	std::unique_ptr<lich::Shader> _shader;
	lich::OrthographicCamera2d _camera;
};

}

#endif
