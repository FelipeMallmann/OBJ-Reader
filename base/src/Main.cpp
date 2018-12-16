//#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

//#include "headers/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/Renderer.h"
#include "headers/VertexBuffer.h"
#include "headers/IndexBuffer.h"
#include "headers/VertexArray.h"
#include "headers/VertexBufferLayout.h"
#include "headers/Shader.h"
#include "headers/Camera.h"

#include "headers/Face.h"
#include "headers/Group.h"
#include "headers/Mesh.h"
#include "headers/Material.h"
#include "headers/OBJReader.h"
#include "headers/Texture.h"
#include "headers/MTLReader.h"

#define pi 3.14159265

void resize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void readBSpline();
Material* getMaterial(vector<Material*> *mat, string materialName);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Lights
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

// camera
float maxY = 0.0f;
float maxZ = 0.0f;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// mouse movements
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// B-Spline
vector<glm::vec3*> bSpline;

GLFWwindow* init() {
	GLFWwindow* window;

	if (!glfwInit()) return NULL;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	// Callbacks
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
		std::cout << "Error to initiate glew!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glewExperimental = GL_TRUE;

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);*/

	return window;
}

int main(void) {
	GLFWwindow* window = init();

	{
		// Read object
		ObjReader obj;	
		Mesh* mesh = obj.read("res/objects/pista.obj");

		ObjReader obj1;
		Mesh* mesa = obj1.read("res/objects/bmw.obj");

		// Criaçao da primeira mesh
		for (int g = 0; g < mesh->groups.size(); g++) {
			vector<float> vs, ts, ns;

			Group* group = mesh->groups[g];
			for (int f = 0; f < group->faces.size(); f++) {

				Face* face = group->faces[f];
				for (int i = 0; i < face->vectors.size(); i++) {
					float vi = face->vectors[i];
					glm::vec3* v = mesh->vertex[vi];  // Trata quando for -1 ?
					vs.push_back(v->x);
					vs.push_back(v->y);
					vs.push_back(v->z);

					//Get max cordinates to adjust camera position
					if (v->y > maxY) maxY = v->y;	
					if (v->z > maxZ) maxZ = v->z;	
				}

				for (int i = 0; i < face->textures.size(); i++) {
					float ti = face->textures[i];
					glm::vec2* t = mesh->maps[ti];
					ts.push_back(t->x);
					ts.push_back(t->y);
				}
				for (int i = 0; i < face->vectors.size(); i++) {
					float ni = face->normals[i];
					glm::vec3* n = mesh->normals[ni];
					ns.push_back(n->x);
					ns.push_back(n->y);
					ns.push_back(n->z);
				}
			}			

			GLuint vao;
			GLuint vbo, vbo2, vbo3;

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vs.size() * sizeof(float), vs.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			
			glGenBuffers(1, &vbo2);
			glBindBuffer(GL_ARRAY_BUFFER, vbo2);
			glBufferData(GL_ARRAY_BUFFER, ts.size() * sizeof(float), ts.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, vbo2);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

			glGenBuffers(1, &vbo3);
			glBindBuffer(GL_ARRAY_BUFFER, vbo3);
			glBufferData(GL_ARRAY_BUFFER, ns.size() * sizeof(float), ns.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, vbo3);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			
			group->SetVAO(vao);
		}

		// Criação da segunda Mesh
		for (int g = 0; g < mesa->groups.size(); g++) {
			vector<float> vs, ts, ns;

			Group* group = mesa->groups[g];
			for (int f = 0; f < group->faces.size(); f++) {

				Face* face = group->faces[f];
				for (int i = 0; i < face->vectors.size(); i++) {
					int vi = face->vectors[i];
					glm::vec3* v = mesa->vertex[vi];
					vs.push_back(v->x);
					vs.push_back(v->y);
					vs.push_back(v->z);

					//Get max cordinates
					if (v->y > maxY)
						maxY = v->y;
					if (v->z > maxZ)
						maxZ = v->z;
				}
				//se for -1 ignora pois nao foi informado
				for (int i = 0; i < face->textures.size(); i++) {
					int ti = face->textures[i];
					if (ti != -1) {
						glm::vec2* t = mesa->maps[ti];
						ts.push_back(t->x);
						ts.push_back(t->y);
					}
				}

				for (int i = 0; i < face->vectors.size(); i++) {
					int ni = face->normals[i];
					if (ni != -1) {
						glm::vec3* n = mesa->normals[ni];
						ns.push_back(n->x);
						ns.push_back(n->y);
						ns.push_back(n->z);
					}
				}
			}

			GLuint vao2;
			GLuint vbo4, vbo5, vbo6;

			glGenVertexArrays(1, &vao2);
			glBindVertexArray(vao2);

			glGenBuffers(1, &vbo4);
			glBindBuffer(GL_ARRAY_BUFFER, vbo4);
			glBufferData(GL_ARRAY_BUFFER, vs.size() * sizeof(float), vs.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			glGenBuffers(1, &vbo5);
			glBindBuffer(GL_ARRAY_BUFFER, vbo5);
			glBufferData(GL_ARRAY_BUFFER, ts.size() * sizeof(float), ts.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, vbo5);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

			glGenBuffers(1, &vbo6);
			glBindBuffer(GL_ARRAY_BUFFER, vbo6);
			glBufferData(GL_ARRAY_BUFFER, ns.size() * sizeof(float), ns.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, vbo6);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			group->SetVAO(vao2);
		}


		// Create shader
		Shader shader("res/shaders/basic.vs", "res/shaders/basic.fs");
		shader.use();

		// Faz a leitura do mtl criando um vetor de materiais
		MtlReader mtl;
		vector<Material*> materials = mtl.read("res/materials/" + mesh->material);

		MtlReader mtl2;
		vector<Material*> materials2 = mtl2.read("res/materials/" + mesa->material);

		// Anda pelos materiais para pegar as texturas
		int textureQtd = 0;
		for (std::vector<Material*>::iterator it = materials.begin(); it != materials.end(); ++it) {
			Texture tempTexture;
			string path = "res/textures/" + (*it)->map;
			std::cout << path << std::endl;

			tempTexture.Load(path, (*it)->map, shader.ID, textureQtd);
			textureQtd += 1;
			(*it)->tid = tempTexture.GetId();
		}

		for (std::vector<Material*>::iterator it = materials2.begin(); it != materials2.end(); ++it) {
			Texture tempTexture;
			string path = "res/textures/" + (*it)->map;
			std::cout << path << std::endl;

			tempTexture.Load(path, (*it)->map, shader.ID, textureQtd);
			textureQtd += 1;
			(*it)->tid = tempTexture.GetId();
		}

		// Fix camera & light initial position
		cameraPos = glm::vec3(0.0f, 100.0f, (maxZ * 2) / 10);
		lightPos = glm::vec3(0.0f, 250.0f, 0.0f);

		// Projection Matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 800.0f);
		shader.setMat4("projection", projection);

		// Send light position as uniform
		shader.setVec3("lightPos", lightPos);

		// Read B-Spline
		readBSpline();

		// Car movements
		glm::vec3 *A, *B;
		int moves = 0;
		int current = 0;
		float w, h;
		float a;
		
		// Game loop
		while (!glfwWindowShouldClose(window)) {

			// Time logic
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Inputs & Callbacks 
			processInput(window);
			glfwSetWindowSizeCallback(window, resize);

			// Clear
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Send data to shader
			shader.use();

			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			shader.setMat4("view", view);

			// PISTA
			glm::mat4 model1 = glm::mat4(1.0f);
			model1 = glm::scale(model1, glm::vec3(0.1f, 0.1f, 0.1f));

			shader.setMat4("model", model1);

			// Send camera position, to make specular illumination//
			shader.setVec3("viewPos", cameraPos);

			// Draws pista
			for (int g = 0; g < mesh->groups.size(); g++) {
				Group* group = mesh->groups[g];

				if (group->material != "") {
					Material *mat = getMaterial(&materials, group->material);
					mat->useTexture();

					shader.setVec3("Ka", mat->Ka);
					shader.setVec3("Kd", mat->Kd);
					shader.setVec3("Ks", mat->Ks);
					shader.setFloat("Ns", mat->Ns);
				}
				shader.use();
				glBindVertexArray(group->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, group->faces.size() * 3);
			}


			// CARRO
			glm::mat4 model2 = glm::mat4(1.0f);

			A = bSpline[current % bSpline.size()];
			B = bSpline[(current + 1) % bSpline.size()];
			moves++;
			if (moves % 6 == 0)
				current++;

			glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(A->x, A->y, A->z));

			w = B->x - A->x;
			h = B->z - A->z;
			a = atan2f(w, h) - pi / 2;
			glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), a, glm::vec3(0.0f, 1.0f, 0.0f));

			w = B->x - A->x;
			h = B->y - A->y;
			a = atan2f(w, h);
			glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), a, glm::vec3(0.0f, 0.0f, 1.0f));

			model2 = T * R1;

			// Draws carro
			for (int g = 0; g < mesa->groups.size(); g++) {
				Group* group = mesa->groups[g];


				shader.setMat4("model", model2);
				shader.use();
				glBindVertexArray(group->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, group->faces.size() * 3);
			}

			// Swaps 
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 0.5;
	//float cameraSpeed = 15.0 * deltaTime * maxZ/5;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void readBSpline()
{
	ifstream arq("pontos.txt");
	while (!arq.eof()) {
		string line;
		std::stringstream sline;
		getline(arq, line);
		sline << line;
		float x, y, z;
		sline >> x >> y >> z;
		glm::vec3 *v = new glm::vec3(x * 0.1, y * 0.1, z * 0.1); // 0.1 Pista scale
		bSpline.push_back(v);
	}
	int i = 0;
}

Material* getMaterial(vector<Material*> *mat, string materialName) {
	for (std::vector<Material*>::iterator it = (*mat).begin(); it != (*mat).end(); ++it) {
		if (materialName == (*it)->name) {
			return *it;
		}
	}
	return NULL;
}