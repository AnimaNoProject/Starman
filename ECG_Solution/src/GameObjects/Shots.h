#pragma once

#include "../Rendering/Model.h";

class Shots
{
private:
	Model* _model;
	vec3 _position;
	vec3 _direction;
	vec3 _startposition;
	mat4 _rotation;
	const unsigned int damage = 2;
public:
	bool _toofar;
	Shots(Model* model, vec3 direction, vec3 position);
	~Shots();
	long draw();
	void update(float deltaTime);
};

