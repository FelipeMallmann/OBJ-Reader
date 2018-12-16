#include "../headers/Mesh.h"

void Mesh::AddGroup(std::string name)
{
	Group *g = new Group(name);
	this->groups.push_back(g);
	currentGroup = g;
}

bool Mesh::GroupExists(std::string name)
{
	for (std::vector<Group*>::iterator it = groups.begin(); it != groups.end(); ++it) {
		if (name == (*it)->name) {

			currentGroup = *it;
			return true;
		}
	}
	currentGroup = NULL;
	return false;
}

Mesh::Mesh()
{
	Group* g = new Group("DEFAULTGROUP");
	groups.push_back(g);
	currentGroup = g;
	//Material materials = material;
}

Mesh::~Mesh()
{
}