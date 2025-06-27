#include "layer.hpp"

namespace lich {

Layer_Stack::Layer_Stack(void):
	_layers{}, _insert{_layers.begin()} {}

Layer_Stack::~Layer_Stack(void) {
	for (auto &layer : _layers) {
		layer->quit();
	}
}

Usize Layer_Stack::push(std::unique_ptr<Layer> layer) {
	_insert = _layers.emplace(_insert, std::move(layer));
	return static_cast<Usize>(std::distance(_layers.begin(), _insert));
}

Usize Layer_Stack::push_over(std::unique_ptr<Layer> layer) {
	_layers.emplace_back(std::move(layer));
	return _layers.size() - 1;
}

std::unique_ptr<Layer> Layer_Stack::remove(Predicate predicate) {
	auto it = std::find_if(_layers.begin(), _layers.end(), predicate);
	if (it == _layers.end()) {
		return nullptr;
	}

	std::unique_ptr<Layer> layer = std::move(*it);
	_layers.erase(it);
	return layer;
}

void Layer_Stack::update(void) {
	for (auto &layer : _layers) {
		layer->update();
	}
}

void Layer_Stack::handle(Event &event) {
	for (auto &layer : _layers | std::views::reverse) {
		layer->handle(event);
		if (event.handled) {
			break;
		}
	}
}

}
