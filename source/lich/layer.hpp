#ifndef LICH_LAYER_HPP
#define LICH_LAYER_HPP

#include "event.hpp"
#include "log.hpp"
#include "util.hpp"

namespace lich {

class Layer {
public:
	Layer(const std::string &name = "Layer"): _name{name} {}
	virtual ~Layer() = default;

	virtual void init() {}
	virtual void quit() {}
	virtual void update([[maybe_unused]] Timestep timestep) {}
	virtual void handle([[maybe_unused]] Event &event) {}

	const std::string &name() const;
	
private:
	std::string _name{};
};

class Layer_Stack {
public:
	using Predicate = std::function<bool(const std::unique_ptr<Layer> &layer)>;
	
	Layer_Stack();
	~Layer_Stack();

	Usize push(std::unique_ptr<Layer> layer);
	Usize push_over(std::unique_ptr<Layer> layer);
	std::unique_ptr<Layer> remove(Predicate predicate);

	void update(Timestep timestep);
	void handle(Event &event);
		
private:
	std::vector<std::unique_ptr<Layer>> _layers{};
	std::vector<std::unique_ptr<Layer>>::iterator _insert{};
};

template<typename Type>
concept Layer_Derived = std::derived_from<Type, Layer>;

}

#endif
