#include "layer.hpp"

namespace lich {

LayerStack::LayerStack():
	_layers{}, _insert{_layers.begin()} {}

LayerStack::~LayerStack()
{
	for (auto &layer : _layers) layer->quit();
}

Usize LayerStack::push(std::unique_ptr<Layer> layer)
{
	layer->init();
	_insert = _layers.emplace(_insert, std::move(layer));
	return static_cast<Usize>(std::distance(_layers.begin(), _insert));
}

Usize LayerStack::push_over(std::unique_ptr<Layer> layer)
{
	layer->init();

	Usize index = std::distance(_layers.begin(), _insert);
	_layers.emplace_back(std::move(layer));
	_insert = _layers.begin() + index;
	
	return _layers.size() - 1;
}

std::unique_ptr<Layer> LayerStack::remove(Predicate predicate)
{
	auto it = std::find_if(_layers.begin(), _layers.end(), predicate);
	if (it == _layers.end()) return nullptr;

	std::unique_ptr<Layer> layer = std::move(*it);
	_layers.erase(it);
	return layer;
}

void LayerStack::update()
{
	for (auto &layer : _layers) layer->update();
}

void LayerStack::handle(Event &event)
{
	for (auto &layer : _layers | std::views::reverse) {
		layer->handle(event);
		if (event.handled) break;
	}
}

}
