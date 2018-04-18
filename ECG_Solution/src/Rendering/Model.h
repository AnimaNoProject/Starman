#pragma once
#ifndef MODEL_H
#define MODEL_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "../Shader/Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
public:
	bool gammaCorrection;
	Model(char *path); // Constructor
	void Draw(Shader shader); // Draw function

private:
	vector<Mesh> meshes; // Model data
	vector<Texture> textures_loaded; // Model data
	string directory; // Directory where the model is stored

	// Basic funtion for processing model
	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	// unsigned int for globally loaded textures. We first check if the texture has already been loaded, because loading texture is pretty expensive
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
};

#endif