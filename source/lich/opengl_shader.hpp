#ifndef LICH_OPENGL_SHADER_HPP
#define LICH_OPENGL_SHADER_HPP

#include <glad/glad.h>

#include "render_shader.hpp"

namespace lich {

class Opengl_Shader final : public Shader {
public:
	static tl::expected<std::unique_ptr<Shader>, std::string>
	compile(const std::string &vertex_source, const std::string &fragment_source);
	
	Opengl_Shader(GLuint program);
	~Opengl_Shader() override;
	void bind() override;
	void unbind() override;
	void upload_uniform(const std::string &name, const glm::mat4 &matrix) override;
	void *handle() const override;
	
private:
	GLuint _program{0};
};

}

#endif
