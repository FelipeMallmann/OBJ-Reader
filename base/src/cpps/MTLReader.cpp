#include "../headers/MTLReader.h"


MtlReader::MtlReader()
{
}


MtlReader::~MtlReader()
{
}

vector<Material*> MtlReader::read(string filename)
{
	vector<Material*>* materials = new vector<Material*>();
	Material *material = NULL;
	ifstream arq(filename);

	while (!arq.eof()) {

		string line;
		string name;
		std::stringstream sline;
		getline(arq, line);

		sline << line;
		string temp;
		sline >> temp;
		if (temp == "newmtl") {
			sline >> name;
			material = new Material(name);
			materials->push_back(material);
		}
		else if (temp == "Ka") {
			float r, g, b;
			sline >> r >> g >> b;
			material->Ka = glm::vec3(r, g, b);
		}
		else if (temp == "Kd") {
			float r, g, b;
			sline >> r >> g >> b;
			material->Kd = glm::vec3(r, g, b);
		}
		else if (temp == "Ks") {
			float r, g, b;
			sline >> r >> g >> b;
			material->Ks = glm::vec3(r, g, b);
		}
		else if (temp == "Ns") {
			float ns;
			sline >> ns;
			material->Ns = ns;
		}
		else if (temp == "map_Kd") {
			std::string map;
			sline >> map;
			material->map = map;
		}
	}
	return *materials;
}