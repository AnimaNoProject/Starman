#pragma once
#include "RUnit.h"
#include "Camera.h"
#include "../Rendering/Model.h"
#include <algorithm>
#include "Light.h"
#include "CollisionData.h"
#include "PlayerConfig.h"

class RPlayer
{
private:

	Model* _model, *_shot;
	_Shader* _shader;
	vec3 _right, _up, _middle;
	CollisionData* _collisionData;

	float timepassedL, timepassedR, _yaw, _pitch;

	const float cooldown = 0.5f;
	const float _mouse_speed = 2.0f;

	void shoot(float deltaTime, bool shootL, bool shootR);
	void InitPhysicProperties(vec3 position);
	void updateShots(int deltaTime);
public:
	// Physics related stuff
	btDynamicsWorld*			_world;
	btRigidBody*				_body;
	btConvexHullShape*			_shape;
	vector<btScalar>			shapeVector;
	vec3 _position, _particleSpawn, _particleSpawnR, _particleSpawnL, _dir;
	vector<Shots*> shots;
	Camera* _camera;

	RPlayer();
	RPlayer(Model* model, Camera* camera, _Shader* shader);
	~RPlayer();

	double _real_speed;
	int _speed, _health;

	void move(float x, float y, bool up, bool down, bool left, bool right, bool shootR, bool shootL, float deltaTime);
	long draw();

};

