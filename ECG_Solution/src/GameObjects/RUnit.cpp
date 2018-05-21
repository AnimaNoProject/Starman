#include "RUnit.h"

RUnit::RUnit(Model * model, vec3 translation, vec3 rotation, float degree, vec3 position, vec3 scaleIt)
{
	_model = model;
	_translation = translation;
	_rotation = rotation;
	_degree = degree;
	_position = translate(mat4(1), position);
	_scale = scale(mat4(1), scaleIt);

	_shape = new btConvexHullShape();
	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < model->meshes.at(i)._vertices.size(); j++)
		{
			btVector3 btv = btVector3(model->meshes.at(i)._vertices.at(j).Position.x, model->meshes.at(i)._vertices.at(j).Position.y, model->meshes.at(i)._vertices.at(j).Position.z);
			((btConvexHullShape*)_shape)->addPoint(btv);
		}
	}

	btQuaternion rotationQuat;
	rotationQuat.setEulerZYX(rotation.x, rotation.y, rotation.z);
	btVector3 positionbT = btVector3(position.x, position.y, position.z);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotationQuat, positionbT));

	btScalar mass = 1500;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(mass, bodyInertia);

	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, _shape, bodyInertia);

	bodyCI.m_restitution = 0.5f;
	bodyCI.m_friction = 0.001f;

	_body = new btRigidBody(bodyCI);

	_body->setLinearFactor(btVector3(1, 1, 0));

	_body->setLinearVelocity(btVector3(translation.x, translation.y, translation.z));
}

RUnit::RUnit(Model * model)
{
	_model = model;
	float px, py, pz, r, tx, ty, tz, rx, ry, rz, s;
	tx = rand() & 1;
	ty = rand() & 1;
	tz = rand() & 1;
	px = rand() % 500;
	py = rand() % 500;
	pz = rand() % 500;

	if ((rand() & 1) == 1)
	{
		px = -px;
	}
	if ((rand() & 1) == 1)
	{
		py = -py;
	}
	if ((rand() & 1) == 1)
	{
		pz = -pz;
	}

	rx = rand() & 1;
	ry = rand() & 1;
	rz = rand() & 1;
	s = rand() % 25;
	r = rand() % 25 / 100;
	_translation = vec3(tx, ty, tz);
	_position = translate(mat4(1), vec3(px, py, pz));
	_rotation = vec3(rx, ry, rz);
	_scale = scale(mat4(1), vec3(s, s, s));
	_degree = r;

	_shape = new btConvexHullShape();
	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < model->meshes.at(i)._vertices.size(); j++)
		{
			btVector3 btv = btVector3(model->meshes.at(i)._vertices.at(j).Position.x, model->meshes.at(i)._vertices.at(j).Position.y, model->meshes.at(i)._vertices.at(j).Position.z);
			((btConvexHullShape*)_shape)->addPoint(btv);
		}
	}

	btQuaternion rotation;
	rotation.setEulerZYX(rz, ry, rx);
	btVector3 position = btVector3(px, py, pz);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));

	btScalar mass = s;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(mass, bodyInertia);

	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, _shape, bodyInertia);

	bodyCI.m_restitution = 0.5f;
	bodyCI.m_friction = 0.001f;

	_body = new btRigidBody(bodyCI);

	_body->setLinearFactor(btVector3(1, 1, 0));

	_body->setLinearVelocity(btVector3(tx, ty, tz));

	_body->setAngularFactor(btVector3(rx, ry, rz));
	_body->setAngularVelocity(btVector3(0.001, 0.001, 0.001));
}

RUnit::RUnit(mat4 defaultTransformation)
{
	_defaultTransformation = defaultTransformation;
}

RUnit::RUnit()
{
}

RUnit::~RUnit()
{
	if (_body)
	{
		delete _body->getMotionState();
		delete _body;
	}
	delete _shape;
}

long RUnit::draw()
{
	unsigned int triangle = 0;
	if (_model != nullptr)
	{
		_model->setTransformMatrix(_transformation);
		triangle += _model->Draw();
	}
	for (int i = 0; i < this->children.size(); i++)
	{
		triangle += this->children.at(i)->draw();
	}
	return triangle;
}

void RUnit::addChild(RUnit* unit)
{
	this->children.push_back(unit);
}

void RUnit::setDefaultTransformation(vec3 translation, vec3 rotation, float degree)
{
	_translation = translation;
	_rotation = rotation;
	_degree = degree;
}

void RUnit::update(mat4 transformation, float time)
{
	if (_model != nullptr)
	{
		btTransform transform = _body->getWorldTransform();
		btQuaternion rota = transform.getRotation();
		_rotation = vec3(rota.getX(), rota.getY(), rota.getZ());
		_degree = rota.getAngle();
		_transformation = translate(mat4(1), vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z())) * rotate(mat4(1), _degree, _rotation) * _scale;
	}

	for (int i = 0; i < this->children.size(); i++)
	{
		this->children.at(i)->update(transformation, time);
	}
}