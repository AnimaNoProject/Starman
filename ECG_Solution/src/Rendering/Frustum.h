#pragma once
#include "glm/glm.hpp"
#include "../Utility/Plane.h"

using namespace glm;

class Frustum
{
private:
	float fov;
	float aspect; 
	float nearZ; 
	float farZ;
	float hNear, hFar, wNear, wFar;
	Plane planes[6];
public:
	Frustum(float fov, float aspect, float nearZ, float farZ);
	~Frustum();
	void calculatePlanes(vec3 const & eye, vec3 const & center, vec3 const & up);
	bool isVisible(vec3 position, float radius);
};

