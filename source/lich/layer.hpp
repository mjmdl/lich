#ifndef LICH_LAYER_HPP
#define LICH_LAYER_HPP

#include "event.hpp"
#include "log.hpp"

namespace lich {

class Layer {
public:
	Layer(const std::string &name = "Layer"): _name{name} {}
	virtual ~Layer(void) = default;

	virtual void init(void) {}
	virtual void quit(void) {}
	virtual void update(void) {}
	virtual void handle(Event &event) { (void)event; }

	inline const std::string &name(void) const { return _name; }
	
private:
	std::string _name;
};

class Layer_Stack {
public:
	using Predicate = std::function<bool(const std::unique_ptr<Layer> &layer)>;
	
	Layer_Stack(void);
	~Layer_Stack(void);

	Usize push(std::unique_ptr<Layer> layer);
	Usize push_over(std::unique_ptr<Layer> layer);
	std::unique_ptr<Layer> remove(Predicate predicate);

	void update(void);
	void handle(Event &event);
		
private:
	std::vector<std::unique_ptr<Layer>> _layers;
	std::vector<std::unique_ptr<Layer>>::iterator _insert;
};

template<typename Type>
concept Layer_Derived = std::derived_from<Type, Layer>;

}

#endif
