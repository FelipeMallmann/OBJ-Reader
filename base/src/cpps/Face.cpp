#include "../headers/Face.h"


void Face::AddIndex(int vector, int texture, int normal) {
	this->vectors.push_back(vector);
	this->textures.push_back(texture);
	this->normals.push_back(normal);
};

Face::Face()
{
}