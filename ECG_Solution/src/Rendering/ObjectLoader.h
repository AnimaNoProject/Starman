#pragma once
#include <glm\glm.hpp>
#include <vector>

class ObjectLoader
{
private:
	std::vector< unsigned int > uvIndices, normalIndices;
public:
	ObjectLoader();
	~ObjectLoader();
	static bool ObjectLoader::loadOBJ(const char * path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, std::vector<unsigned int>& out_indices);
};

