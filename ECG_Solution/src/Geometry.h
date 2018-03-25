/*
* Copyright 2017 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once


#include <vector>
#include <memory>
#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>




struct GeometryData {
	std::vector<glm::vec3> positions;
	std::vector<unsigned int> indices;
};


class Geometry
{
protected:

	GLuint _vao;
	GLuint _vboPositions;
	GLuint _vboIndices;
	unsigned int _elements;

public:
	Geometry(GeometryData& data);

	~Geometry();
	void draw();


	static GeometryData createTestObject(float width, float height, float depth);
};