#pragma once
#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL\glew.h>

#include "../Shader/Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

// Struct for everything that is related to a vertex
struct Vertex {
	glm::vec3 Position; // Vector that stores the position of a vertex
	glm::vec3 Normal; // Vector that stores the normal of a vertex
	glm::vec2 TexCoords; // Vector to store the texture coordinates
	glm::vec3 Tangent; // Vector to store the tangents
	glm::vec3 Bitangent; // Vector to store the bitangents
};

// Struct for everything that is related to the texture of a mesh
struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	//  Mesh Data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	// Constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	// render the mesh
	void Draw(Shader shader);

private:
	// VBO and EBO for rendering
	unsigned int VBO, EBO;

	// initializes all the buffer objects/arrays
	void setupMesh();
};
#endif

