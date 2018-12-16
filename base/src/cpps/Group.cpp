#include "../headers/Group.h"


void Group::AddFace(Face* f)
{
	this->faces.push_back(f);
}

Group::Group()
{

}

Group::Group(std::string name)
{
	this->name = name;
}

Group::~Group()
{
}