#include "Frustum.h"

#define ANG2RAD 3.14159265358979323846/180.0

Frustum::Frustum(float fov, float aspect, float nearZ, float farZ) : _fov(fov), _aspect(aspect), _nearZ(nearZ), _farZ(farZ)
{
	_tang = (float)tan(fov* ANG2RAD * 0.5);
	nh = nearZ * _tang;
	nw = nh * aspect;
	fh = farZ * _tang;
	fw = fh * aspect;
}

Frustum::~Frustum()
{
}

void Frustum::Update(vec3 eye, vec3 dir, vec3 up)
{
	vec3  nc, fc, X, Y, Z;

	Z = normalize(eye - dir);

	X = normalize(up * Z);

	Y = Z * X;

	nc = eye - Z * _nearZ;
	fc = eye - Z * _farZ;

	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;

	planes[TOP].setPoints(ntr, ntl, ftl);
	planes[BOTTOM].setPoints(nbl, nbr, fbr);
	planes[LEFT].setPoints(ntl, nbl, fbl);
	planes[RIGHT].setPoints(nbr, ntr, fbr);
	planes[NEAR].setPoints(ntl, ntr, nbr);
	planes[FAR].setPoints(ftr, ftl, fbl);
}

bool Frustum::Inside(vec3 point, float radius)
{
	//return true;
	float distance = 0;
	for (int i = 0; i < 6; i++) {

		distance = planes[i].distance(point);

		if (distance < -radius)
		{
			return false;
		}
		else if (distance < radius)
		{
			return true;
		}
	}
	return true;
}
