#include <glad/glad.h>
#include <tl/expected.hpp>

#include "log.hpp"
#include "opengl_render.hpp"

namespace lich {

void pop_opengl_errors() {
	GLenum error;
	do error = glGetError(); while (error != GL_NO_ERROR);
}

void check_opengl_error(const char *file, int line) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		log_error("OpenGL error at file {} line {}: #{}.", file, line, error);
	}
}

void assert_opengl_error(const char *file, int line) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		log_fatal("OpenGL error at file {} line {}: #{}.", file, line, error);
		LICH_ABORT();
	}
}

void Opengl_Renderer_Api::set_clear_color(const glm::vec4 &color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Opengl_Renderer_Api::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Opengl_Renderer_Api::
draw_indexed(const std::unique_ptr<Vertex_Array> &vertex_array) {
	if (vertex_array->index_buffer()) {
		glDrawElements(
			GL_TRIANGLES,
			vertex_array->index_buffer()->count(),
			GL_UNSIGNED_INT,
			NULL
		);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, vertex_array->vertex_count());
	}
}

}
