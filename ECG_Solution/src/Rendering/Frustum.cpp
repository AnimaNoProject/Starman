#include "Frustum.h"

#define ANG2RAD 3.14159265358979323846/180.0

Frustum::Frustum(float fov, float aspect, float nearZ, float farZ) : _fov(fov), _aspect(aspect), _nearZ(nearZ), _farZ(farZ)
{
	_tang = (float)tan(fov* ANG2RAD * 0.5);
	nh = nearZ * _tang;
	nw = nh * aspect;
	fh = farZ * _tang;
	fw = fh * aspect;
	_on = false;
}

Frustum::~Frustum()
{
}

void Frustum::Update(vec3 eye, vec3 dir, vec3 up, bool on)
{
	_on = on;
	if (!_on)
		return;

	vec3  nc, fc, X, Y, Z;

	Z = normalize(eye - dir);

	X = normalize(up * Z);

	Y = Z * X;

	nc = eye - Z * _nearZ;
	fc = eye - Z * _farZ;

	planes[NEAR].setUp(-Z, nc);
	planes[FAR].setUp(Z, fc);

	vec3 aux, normal;

	aux = (nc + Y * nh) - eye;
	aux = normalize(aux);
	normal = aux * X;
	planes[TOP].setUp(normal, nc + Y * nh);

	aux = (nc - Y * nh) - eye;
	aux = normalize(aux);
	normal = X * aux;
	planes[BOTTOM].setUp(normal, nc - Y * nh);

	aux = (nc - X * nw) - eye;
	aux = normalize(aux);
	normal = aux * Y;
	planes[LEFT].setUp(normal, nc - X * nw);

	aux = (nc + X * nw) - eye;
	aux = normalize(aux);
	normal = Y * aux;
	planes[RIGHT].setUp(normal, nc + X * nw);
}

bool Frustum::Inside(vec3 point, float radius)
{
	if (!_on)
		return true;

	float distance = 0;
	for (int i = 0; i < 6; i++) {

		distance = planes[i].distance(point);

		if (distance < -radius)
		{
			return false;
		}
	}
	return true;
}
