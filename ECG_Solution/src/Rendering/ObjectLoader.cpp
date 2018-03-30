#include "ObjectLoader.h"



ObjectLoader::ObjectLoader()
{
}


ObjectLoader::~ObjectLoader()
{
}

bool ObjectLoader::loadOBJ(const char * path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, std::vector<unsigned int>& out_indices)
{
	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			out_vertices.push_back(vertex);
		} 
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			out_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			out_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[9];
			//int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2], &vertexIndex[3], &vertexIndex[4], &vertexIndex[5]);
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2], &vertexIndex[3], &vertexIndex[4], &vertexIndex[5], &vertexIndex[6], &vertexIndex[7], &vertexIndex[8]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			out_indices.push_back(vertexIndex[0]);
			out_indices.push_back(vertexIndex[1]);
			out_indices.push_back(vertexIndex[2]);
			out_indices.push_back(vertexIndex[3]);
			out_indices.push_back(vertexIndex[4]);
			out_indices.push_back(vertexIndex[5]);
			out_indices.push_back(vertexIndex[6]);
			out_indices.push_back(vertexIndex[7]);
			out_indices.push_back(vertexIndex[8]);
			/*
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);*/
		}
	}

	return true;
}
