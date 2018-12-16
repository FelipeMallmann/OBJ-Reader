#pragma once

#include <string>

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>

using namespace std;

class Material
{
public:
	glm::vec3 Ka, Kd, Ks;
	float Ns;
	std::string name, map;
	unsigned int tid;

	Material();
	Material(string name);
	void useTexture();
	~Material();
};