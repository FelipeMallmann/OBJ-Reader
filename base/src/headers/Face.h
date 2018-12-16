#pragma once

#include <vector>

//using
using namespace std;

class Face
{
public:
	vector<int> vectors;
	vector<int> normals;
	vector<int> textures;

	void AddIndex(int vector, int texture, int normal);
	Face();
};