#pragma once

#include "Mesh.h"
#include <string>
#include <iostream>
#include <fstream> 
#include <sstream>
#include <vector>

#include "Material.h"

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace std;

class MtlReader
{
public:
	MtlReader();
	~MtlReader();
	vector<Material*> read(string filename);
};