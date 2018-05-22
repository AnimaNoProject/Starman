#include "Shots.h"

Shots::Shots(Model* model, vec3 direction, vec3 position)
{
	_model = model;

	_toofar = false;

	// set starting position
	_direction = direction;
	_position = position;
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