#ifndef SAND_RENDER_LAYER_HPP
#define SAND_RENDER_LAYER_HPP

#include <lich/layer.hpp>
#include <lich/render.hpp>

namespace sand {

class RenderLayer final: public lich::Layer {
public:
	RenderLayer();
	void update() override;

private:
	std::unique_ptr<lich::VertexArray> _vertex_array;
	std::unique_ptr<lich::Shader> _shader;
};

}

#endif
