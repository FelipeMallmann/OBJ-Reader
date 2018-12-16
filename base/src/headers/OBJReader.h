#pragma once

#include "Mesh.h"
#include <string>
#include <iostream>
#include <fstream> 
#include <sstream>

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace std;

class ObjReader
{
public:
	ObjReader();
	~ObjReader();
	Mesh *read(string filename);
};