#pragma once

#include "glm/glm.hpp"

using namespace glm;

struct Plane
{
	vec3 normal;
	vec3 point;
	float d;

	void setUp(vec3 normal0, vec3 vertex0)
	{
		normal = normalize(normal0);
		point = vertex0;
		d = -(normal.x * point.x + normal.y * point.y + normal.z * point.z);
	};

	float distance(vec3 target)
	{
		return d + (normal.x * target.x + normal.y * target.y + normal.z * target.z);
	};

	void setPoints(vec3 v1, vec3 v2, vec3  v3)
	{
		vec3 aux1, aux2;

		aux1 = v1 - v2;
		aux2 = v3 - v2;

		normal = normalize(cross(aux2, aux1));
		point = v2;
		d = -(normal.x * point.x + normal.y * point.y + normal.z * point.z);
	};
};


class Frustum
{
private:
	Plane planes[6];
	float _fov, _aspect, _nearZ, _farZ, _tang;
	float nw, nh, fw, fh;
	vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	enum { TOP = 0, BOTTOM, RIGHT, LEFT, NEAR, FAR };
	bool _on;
public:
	Frustum(float fov, float aspect, float nearZ, float farZ);
	~Frustum();
	void Update(vec3 eye, vec3 dir, vec3 up, bool on);
	bool Inside(vec3 point, float radius);
};

