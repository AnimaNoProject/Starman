#include "Shots.h"

Shots::Shots(Model* model, vec3 direction, vec3 position)
{
	_model = model;
	_direction = direction;
	_position = position;
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
	_position += _direction * (float)(deltaTime * 50.0f);
	//_model->setTransformMatrix(translate(mat4(1), _position));
}
