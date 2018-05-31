#pragma once
#include "RUnit.h"
#include "Camera.h"
#include "../Rendering/Model.h"
#include <algorithm>
#include "Light.h"

class RPlayer
{
private:
	vec3 _position;
	Model* _model, *_shot;
	_Shader* _shader;

	vec3 _right, _up, _dir;

	float timepassedL, timepassedR, _yaw, _pitch;

	const float cooldown = 0.25f;
	const float _mouse_speed = 2.0f;

	void shoot(float deltaTime, bool shootL, bool shootR);
	void collisionCheck();
	void InitPhysicProperties(vec3 position);
public:
	// Physics related stuff
	btDynamicsWorld * _world;
	btRigidBody * _body;
	btConvexHullShape* _shape;
	vector<btScalar> shapeVector;


	vector<Shots*> shots;
	Camera* _camera;
	vec3 getPosition() { return _position; }
	SpotLight* spotLight;

	RPlayer();
	RPlayer(Model* model, Camera* camera, _Shader* shader);
	~RPlayer();

	double _real_speed;
	int _speed, _health;

	void move(float x, float y, bool up, bool down, bool left, bool right, bool shootR, bool shootL, float deltaTime);
	void addToPhysics();
	long draw();

};

