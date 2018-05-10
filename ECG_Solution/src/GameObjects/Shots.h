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
	mat4 lookAt(vec3 const & eye, vec3 const & center, vec3 const & up);
	const unsigned int damage = 2;
public:
	bool _toofar;
	Shots(Model* model, vec3 direction, vec3 position, vec3 up, vec3 right);
	~Shots();
	long draw();
	void update(float deltaTime);
};

