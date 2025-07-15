#ifndef LICH_UTIL_HPP
#define LICH_UTIL_HPP

namespace lich {

class Timestep {
public:
	Timestep(float time = 0.0f) : _time{time} {}

	float seconds() const { return _time; }
	float miliseconds() const { return _time * 1'000.0f; }
	
private:
	float _time;
};

}

#endif
