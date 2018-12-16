#include "../headers/Texture.h"

Texture::Texture()
{
}


Texture::~Texture()
{
}


void Texture::Load(std::string path, std::string textureUniformName, GLuint shaderProgram, GLint textureNum)
{
	int width, height;
	unsigned char *image;

	glGenTextures(1, &tid);

	char* pathChar = const_cast<char*>(path.c_str());
	char* name = const_cast<char*>(textureUniformName.c_str());

	GLint location = glGetUniformLocation(shaderProgram, "texture1");

	glActiveTexture(GL_TEXTURE0 + (tid - 1));
	glBindTexture(GL_TEXTURE_2D, tid);

	LoadImage(pathChar);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniform1i(location, (tid - 1));

	glBindTexture(GL_TEXTURE_2D, 0);
}