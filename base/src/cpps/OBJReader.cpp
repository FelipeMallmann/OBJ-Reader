#include "../headers/OBJReader.h"

Mesh * ObjReader::read(string filename) {
	Mesh *mesh = new Mesh();
	ifstream arq(filename);

	glm::vec3 *v, *vn;
	glm::vec2 *vt;

	while (!arq.eof()) {
		string line;
		std::stringstream sline;
		getline(arq, line);

		sline << line;
		string temp;
		sline >> temp;

		// vertice
		if (temp == "v") {
			float x, y, z;
			sline >> x >> y >> z;
			v = new glm::vec3(x, y, z);
			mesh->vertex.push_back(v);
		}

		// texture
		else if (temp == "vt") {
			float x, y;
			sline >> x >> y;
			vt = new glm::vec2(x, y);
			mesh->maps.push_back(vt);
		}

		// normals
		else if (temp == "vn") {
			float x, y, z;
			sline >> x >> y >> z;
			vn = new glm::vec3(x, y, z);
			mesh->normals.push_back(vn);
		}

		// group
		else if (temp == "g") {
			std::string name;
			std::getline(sline, name);

			if (!mesh->GroupExists(name))
				mesh->AddGroup(name);		
		}

		// faces
		else if (temp == "f") {
			if (mesh->currentGroup == NULL)
				mesh->GroupExists("DEFAULTGROUP");

			int count = 0;

			std::string token;
			std::string auxString;
			Face* face = new Face();

			while (!sline.eof()) {
				sline >> token;
				std::stringstream sToken;
				sToken << token;
				int vi, ti, ni = 0;

				std::getline(sToken, auxString, '/');
				vi = stoi(auxString);
				count++;
				if (!sToken.eof()) {
					std::getline(sToken, auxString, '/');
					if (auxString != "")
						ti = stoi(auxString);
					if (!sToken.eof()) {
						std::getline(sToken, auxString, '/');
						ni = stoi(auxString);
					}
				}

				if (count <= 3) {
					face->AddIndex(--vi, --ti, --ni);
				}
				else {
					//tem mais de 3 
					Face* faceAux = new Face;
					faceAux->AddIndex(face->vectors[0], face->textures[0], face->normals[0]);
					faceAux->AddIndex(face->vectors[2], face->textures[2], face->normals[2]);
					faceAux->AddIndex(--vi, --ti, --ni);
					mesh->currentGroup->AddFace(faceAux);
				}
			}
			mesh->currentGroup->AddFace(face);
		}
		else if (temp == "usemtl") {
			std::string mtlName;
			sline >> mtlName;

			mesh->currentGroup->material = mtlName;
		}
		else if (temp == "mtllib") {
			std::string mtllibName;
			sline >> mtllibName;
			mesh->material = mtllibName;
		}

	}
	arq.close();
	return mesh;
}

ObjReader::ObjReader()
{
}

ObjReader::~ObjReader()
{
}