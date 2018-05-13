#include "Plane.h"

Plane::Plane(glm::vec3 &normal, glm::vec3 &point)
{
	this->normal = normalize(normal);
	d = -(normal.x * point.x, normal.y * point.y, normal.z * point.z);
}

Plane::Plane()
{
}

Plane::~Plane()
{
}

float Plane::distance(glm::vec3 point)
{
	return d + (normal.x * point.x + normal.y * point.y + normal.z * point.z);
}
