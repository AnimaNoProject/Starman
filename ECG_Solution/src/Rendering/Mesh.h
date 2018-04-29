#pragma once
#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL\glew.h>
#include "../Shader/_Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;
using namespace glm;

// Struct for everything that is related to a vertex
struct Vertex {
	vec3 Position; // Vector that stores the position of a vertex
	vec3 Normal; // Vector that stores the normal of a vertex
	vec2 TexCoords; // Vector to store the texture coordinates
};

// Struct for everything that is related to the texture of a mesh
struct Texture {
	unsigned int id;
	string type;
	string path;
	float Ka;
	float Kd;
	float Ks;
	float Alpha;
};

class Mesh {
public:
	//  Mesh Data
	vector<Vertex> _vertices;
	vector<unsigned int> _indices;
	vector<Texture> _textures;
	unsigned int VAO;

	// Constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, _Shader* shader);

	// Method for rendering the models/meshes
	void Draw();
	// Method for setting the transformation matrix of the mesh
	void setTransformMatrixForMesh(mat4 transformMatrix);
	// Method to reset the transformation matrix of the mesh
	void resetModelMatrixForMesh();
	// Method for transforming the mesh
	void transformMesh(glm::mat4 transformation);

private:
	// VBO and EBO for rendering
	unsigned int _VBO, _EBO;

	// Model & Transformation matrix
	mat4 _modelMatrix;
	mat4 _transformMatrix;

	// Shader
	_Shader* _shader;

	// Method for initializing all the buffer objects/arrays
	void setupMesh();
};
#endif

