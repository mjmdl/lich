#ifndef LICH_RENDER_SHADER_HPP
#define LICH_RENDER_SHADER_HPP

#include <glm/glm.hpp>
#include <tl/expected.hpp>

namespace lich {

enum class Shader_Data_Type {
	None = 0,
	Bool,
	Int,
	Int2,
	Int3,
	Int4,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
};

class Shader {
public:
	static tl::expected<std::unique_ptr<Shader>, std::string>
	create(const std::string &vertex_source, const std::string &fragment_source);

	virtual ~Shader() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void *handle() const = 0;
	virtual void upload_uniform(const std::string &name, const glm::mat4 &matrix) = 0;
};

Usize component_count_of(Shader_Data_Type type);
Usize size_of(Shader_Data_Type type);

}

#endif
