#include "layer.hpp"

namespace lich {

const std::string &Layer::name() const { return _name; }

Layer_Stack::Layer_Stack() : _layers{}, _insert{_layers.begin()} {}

Layer_Stack::~Layer_Stack() { for (auto &layer : _layers) layer->quit(); }

Usize Layer_Stack::push(std::unique_ptr<Layer> layer) {
	layer->init();
	_insert = _layers.emplace(_insert, std::move(layer));
	return static_cast<Usize>(std::distance(_layers.begin(), _insert));
}

Usize Layer_Stack::push_over(std::unique_ptr<Layer> layer) {
	layer->init();

	Usize index = std::distance(_layers.begin(), _insert);
	_layers.emplace_back(std::move(layer));
	_insert = _layers.begin() + index;
	
	return _layers.size() - 1;
}

std::unique_ptr<Layer> Layer_Stack::remove(Predicate predicate) {
	auto it = std::find_if(_layers.begin(), _layers.end(), predicate);
	if (it == _layers.end()) return nullptr;

	std::unique_ptr<Layer> layer = std::move(*it);
	_layers.erase(it);
	return layer;
}

void Layer_Stack::update(Timestep timestep) {
	for (auto &layer : _layers) layer->update(timestep);
}

void Layer_Stack::handle(Event &event) {
	for (auto &layer : _layers | std::views::reverse) {
		layer->handle(event);
		
		if (event.handled) {
			log_trace("Event handled: {}", event.string());
			break;
		}
	}
}

}
