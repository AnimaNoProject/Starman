#include "Frustum.h"

#define ANG2RAD 3.14159265358979323846/180.0

Frustum::Frustum(float fov, float aspect, float nearZ, float farZ)
{
	this->fov = fov;
	this->aspect = aspect;
	this->nearZ = nearZ;
	this->farZ = farZ;

	float tang = (float)tan(ANG2RAD * fov * 0.5);
	hNear = nearZ * tang;
	wNear = hNear * aspect;
	hFar = farZ * tang;
	wFar = hFar * aspect;
}


Frustum::~Frustum()
{
}

void Frustum::calculatePlanes(vec3 const & eye, vec3 const & center, vec3 const & up)
{
	vec3 x, y, z, nearCenter, farCenter, aux, normal;

	z = normalize(eye - center);
	x = normalize(up * z);
	y = z * x;
	nearCenter = eye - z * nearZ;
	farCenter = eye - z * farZ;

	planes[0] = Plane(-z, nearCenter);
	planes[1] = Plane(z, farCenter);

	aux = normalize((nearCenter + y * hNear) - eye);
	normal = aux * x;

	planes[2] = Plane(normal, nearCenter + y * hNear);

	aux = normalize((nearCenter - y * hNear) - eye);
	normal = x * aux;
	
	planes[3] = Plane(normal, nearCenter - y * hNear);

	aux = normalize((nearCenter - x * wNear) - eye);
	normal = aux * y;

	planes[4] = Plane(normal, nearCenter - x * wNear);

	aux = normalize((nearCenter + x * wNear) - eye);
	normal = y * aux;

	planes[5] = Plane(normal, nearCenter + x * wNear);
}

bool Frustum::isVisible(vec3 position, float radius)
{
	float distance;

	for (int i = 0; i < 6; i++)
	{
		distance = planes[i].distance(position);
		if (distance < -radius || distance < radius)
			return true;
	}
	return false;
}