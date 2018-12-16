#include "../headers/Material.h"

Material::Material()
{
}

Material::Material(std::string name)
{
	this->name = name;
}

void Material::useTexture()
{
	//glActiveTexture(GL_TEXTURE0 + (this->tid - 1));
	glBindTexture(GL_TEXTURE_2D, this->tid);
}

Material::~Material()
{
}