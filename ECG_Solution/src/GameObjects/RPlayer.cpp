#include "RPlayer.h"

RPlayer::RPlayer()
{
}

RPlayer::RPlayer(Model* model, Camera* camera, _Shader* shader) : _speed(0), _real_speed(0), _health(100), _yaw(0.0f), _pitch(0.0f)
{
	_model = model;
	_model->transform(translate(mat4(1), vec3(0.0f, -1.5f, 5.5f)));
	_camera = camera;
	_shader = shader;
	_position = vec3(0.0f, 0.0f, 5.0f);
	_shot = new Model("assets/objects/starman_ship/shots.obj", _shader, false);
	timepassedR = timepassedL = 0;
	InitPhysicProperties(_position);
}

RPlayer::~RPlayer()
{
}

void RPlayer::InitPhysicProperties(vec3 position)
{
	// convex hull shape
	for (unsigned int i = 0; i < _model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < _model->meshes.at(i)._vertices.size(); j++)
		{
			vec4 tempPos = vec4(_model->meshes.at(i)._vertices.at(j).Position, 1.0);
			tempPos = translate(mat4(1), vec3(0.0f, -1.5f, 5.5f)) * tempPos;
			shapeVector.push_back(tempPos.x);
			shapeVector.push_back(tempPos.y);
			shapeVector.push_back(tempPos.z);
		}
	}
	_shape = new btConvexHullShape(&shapeVector[0], shapeVector.size() / 3, 3 * sizeof(btScalar));
	//

	// Motion State
	btQuaternion rotationQuat;
	rotationQuat.setEulerZYX(0, 0, 0);
	btVector3 positionbT = btVector3(position.x, position.y, position.z);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotationQuat, positionbT));
	//

	// Weight
	btScalar mass = 5000;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(mass, bodyInertia);
	//

	// Rigid Body
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, _shape, bodyInertia);
	bodyCI.m_restitution = 1.0f;
	bodyCI.m_friction = 0.5f;
	_body = new btRigidBody(bodyCI);

	_collisionData = new CollisionData(PLAYER);
	_collisionData->_parentPlayer = this;
	_body->setUserPointer(_collisionData);
}

void RPlayer::move(float x, float y, bool up, bool down, bool left, bool right, bool shootL, bool shootR, float deltaTime)
{
	if (up)
		(_real_speed >= 50) ? _real_speed = 50 : _real_speed += 5 * deltaTime;
	else if (down)
		(_real_speed <= -50) ? _real_speed = -50 : _real_speed -= 5 * deltaTime;

	_speed = (floor((_real_speed * 2) + 0.5) / 2);

	_yaw += _mouse_speed * deltaTime * x;
	_pitch += _mouse_speed * deltaTime * y;

	_dir = vec3(
		cos(_pitch) * sin(_yaw),
		sin(_pitch),
		cos(_pitch) * cos(_yaw)
	);

	_right = vec3(
		sin(_yaw - pi<float>() / 2),
		0,
		cos(_yaw - pi<float>() / 2)
	);

	_up = vec3(glm::cross(_right, _dir));

	_position += _dir * (float)(deltaTime * _speed);

	if (right)
		_position += _right * (float)(deltaTime * _speed);
	else if (left)
		_position -= _right * (float)(deltaTime * _speed);

	_model->setTransformMatrix(translate(_position) * rotate(_yaw, vec3(0.0f, 1.0f, 0.0f)) * rotate(-_pitch, vec3(1.0f, 0.0f, 0.0f)));
		
	_camera->_speed = _speed;
	_camera->update(x, y, up, down, left, right, deltaTime);

	updateShots(deltaTime);
	if(shootL || shootR)
		shoot(deltaTime, shootL, shootR);

	// UPDATE WORLDTRANSFORM
	btTransform transform = btTransform();
	btQuaternion rotation = btQuaternion();
	rotation.setEuler(_yaw, -_pitch, 0.0f);
	transform.setRotation(rotation);
	transform.setOrigin(btVector3(_position.x, _position.y, _position.z));
	_body->setWorldTransform(transform);
	//
}

void RPlayer::updateShots(int deltaTime)
{
	for (int i = 0; i < this->shots.size(); i++)
	{
		this->shots.at(i)->update(deltaTime);
	}


	for (int i = shots.size() - 1; i >= 0; i--)
	{
		if (shots.at(i)->_toofar || shots.at(i)->_collisionFlag)
		{
			_world->removeRigidBody(shots.at(i)->_body);
			shots.erase(shots.begin() + i);
		}
	}
}

void RPlayer::shoot(float deltaTime, bool shootL, bool shootR)
{
	timepassedL += deltaTime;
	timepassedR += deltaTime;

	vec3 dir = normalize(_dir);
	vec3 up = normalize(_up);
	vec3 right = normalize(_right);

	if (timepassedL > cooldown && shootL)
	{
		Shots* leftShot1 = new Shots(_shot, dir, _position + (10.0f*dir - 3.2f*up + 4.5f*right));
		Shots* leftShot2 = new Shots(_shot, dir, _position + (10.0f*dir - 1.5f*up + 4.5f*right));
		_world->addRigidBody(leftShot1->_body);
		_world->addRigidBody(leftShot2->_body);
		shots.push_back(leftShot1);
		shots.push_back(leftShot2);
		timepassedL = 0;
	}

	if (timepassedR > cooldown && shootR)
	{
		Shots* rightShot1 = new Shots(_shot, dir, _position + (10.0f*dir - 3.2f*up - 4.5f*right));
		Shots* rightShot2 = new Shots(_shot, dir, _position + (10.0f*dir - 1.5f*up - 4.5f*right));
		_world->addRigidBody(rightShot1->_body);
		_world->addRigidBody(rightShot2->_body);
		shots.push_back(rightShot1);
		shots.push_back(rightShot2);
		timepassedR = 0;
	}
}

long RPlayer::draw()
{
	long triangles = 0;
	_model->Draw();
	for (int i = 0; i < this->shots.size(); i++)
	{
		if (this->shots.at(i) != nullptr)
			triangles += this->shots.at(i)->draw();
	}
	return triangles;
}
