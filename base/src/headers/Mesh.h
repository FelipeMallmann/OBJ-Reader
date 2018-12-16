#pragma once
#include <vector>
#include "Group.h"


// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

//using
using namespace std;

class Mesh
{
public:
	vector<glm::vec2*> maps; //(DEPOIS COLOCAR O INDEX NOS VETORES DA FACE)
	vector<glm::vec3*> vertex;
	vector<glm::vec3*> normals;
	Group *currentGroup;
	//mtllib string;

	std::string material;
	vector<Group*> groups;
	void AddGroup(std::string name);
	bool GroupExists(std::string name);
	Mesh();
	~Mesh();
};