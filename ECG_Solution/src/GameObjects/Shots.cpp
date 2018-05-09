#include "Shots.h"

Shots::Shots(Model* model, vec3 direction, vec3 position, vec3 up, vec3 right)
{
	_model = model;
	_direction = direction;
	_position = position;
	_position += direction * 2.0f;
	_position -= up * 2.0f;
	_position -= right * 2.0f;
	_startposition = _position;
	_toofar = false;
}

Shots::~Shots()
{
}

void Shots::draw()
{
	_model->setTransformMatrix(translate(mat4(1), _position));
	_model->Draw();
}

void Shots::update(float deltaTime)
{
	_position += _direction * (float)(deltaTime * 100.0f);
	if (distance(_startposition, _position) > 500)
	{
		_toofar = true;
	}
}
