#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>

#include <string>

class Texture
{
private:
	GLuint tid;
	GLuint location;
	GLushort textureNum;

public:
	Texture();
	~Texture();

	//salva a textura
	static unsigned char* LoadImage(char* path) {
		int width, height;
		unsigned char *image;

		image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		return nullptr;
	}
	void Load(std::string path, std::string textureUniformName, GLuint shaderProgram, GLint textureNum);

	GLuint GetId() { return tid; }
	GLshort GetTextureNum() { return textureNum; }

};