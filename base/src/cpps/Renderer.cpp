#include <iostream>
#include "../headers/Renderer.h"

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::DrawArrays(GLenum mode, int count, const VertexArray& vao, const Shader& shader) const {
	shader.use();
	vao.Bind();

	GLCall(glDrawArrays(mode, 0, count));
}

void Renderer::DrawElements(GLenum mode, const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const {
	shader.use();
	vao.Bind();
	ibo.Bind();

	GLCall(glDrawElements(mode, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
}