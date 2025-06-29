#ifndef LICH_RENDERER_HPP
#define LICH_RENDERER_HPP

namespace lich {

class Shader {
public:
	Shader(const std::string &vertex_code, const std::string &fragment_code);
	~Shader();

	void bind();

private:
	U32 _id;
};

}

#endif
