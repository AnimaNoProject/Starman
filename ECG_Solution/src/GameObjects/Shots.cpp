#include "Shots.h"

Shots::Shots(Model* model, vec3 direction, vec3 position, vec3 up, vec3 right)
{
	_model = model;

	_toofar = false;

	// set starting position
	_direction = direction;
	_position = position;
	_position += direction * 2.0f;
	_position -= up * 2.0f;
	_position -= right * 2.0f;

	_startposition = _position;

	// rotate into shooting direction
	vec3 dirA(0.0f, 1.0f, 0.0f);
	vec3 dirB(direction);
	float angle = acos(dot(dirA, dirB));
	_rotation = rotate(mat4(1), angle, cross(dirA, dirB));
}

Shots::~Shots()
{
}

long Shots::draw()
{
	_model->setTransformMatrix(translate(mat4(1), _position) * _rotation);
	return _model->Draw();
}

void Shots::update(float deltaTime)
{
	_position += _direction * (float)(deltaTime * 100.0f);
	if (distance(_startposition, _position) > 500)
	{
		_toofar = true;
	}
}


mat4 Shots::lookAt(vec3 const & eye, vec3 const & center, vec3 const & up)
{
	vec3  f = normalize(center - eye);
	vec3  u = normalize(up);
	vec3  s = normalize(cross(f, u));
	u = cross(s, f);

	mat4 result(1);
	result[0][0] = s.x;
	result[1][0] = s.y;
	result[2][0] = s.z;
	result[0][1] = u.x;
	result[1][1] = u.y;
	result[2][1] = u.z;
	result[0][2] = -f.x;
	result[1][2] = -f.y;
	result[2][2] = -f.z;
	result[3][0] = -dot(s, eye);
	result[3][1] = -dot(u, eye);
	result[3][2] = dot(f, eye);
	return result;
}