#pragma once

#include "../Rendering/Model.h";

class Shots
{
private:
	Model* _model;
	vec3 _position;
	vec3 _direction;
public:
	Shots(Model* model, vec3 direction, vec3 position, vec3 up, vec3 right);
	~Shots();
	void draw();
	void update(float deltaTime);
};

