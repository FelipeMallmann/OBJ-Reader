#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <vector>
#include "Face.h"

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

//using
using namespace std;

class Group
{
private:
	GLuint vao;

public:
	vector<Face*> faces;
	std::string name;
	std::string material;
	
	void AddFace(Face* f);
	Group();
	Group(std::string name);
	~Group();
	void SetVAO(GLuint vao) { this->vao = vao; }
	GLuint GetVAO() { return this->vao; }
};