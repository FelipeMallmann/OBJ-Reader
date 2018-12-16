#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
	void Clear() const;
	void DrawArrays(GLenum mode, int count, const VertexArray& vao, const Shader& shader) const;
	void DrawElements(GLenum mode, const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
};