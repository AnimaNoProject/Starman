#include "RUnit.h"
RUnit::RUnit(Model * model, vec3 position, vec3 translation, vec3 rotation, float degree, vec3 scale, float weight, TYPE type)
{
	_model = model;
	_scale = glm::scale(mat4(1), scale);
	_scaleFactor = scale.x;
	_type = type;
	// Init Physics
	InitPhysicProperties(position, translation, rotation, degree, _scale, weight);
}

RUnit::RUnit(Model * model, TYPE type)
{
	_type = type;
	_model = model;
	InitRandom();
}

void RUnit::InitRandom()
{
	// Generate Random Properties
	vec3 position(Random::randomNumber(-5001, 5001), Random::randomNumber(-5001, 5001), Random::randomNumber(-5001, 5001));
	vec3 translation(Random::randomNumber(-10, 10), Random::randomNumber(-10, 10), Random::randomNumber(-10, 10));
	vec3 rotation(Random::randomNumber(0, 1), Random::randomNumber(0, 1), Random::randomNumber(0, 1));
	float scaleFactor = Random::randomNumber(1, 225);
	float degree = Random::randomNumber(1, 45);
	float weight = scaleFactor * 100;
	_scaleFactor = scaleFactor;
	//
	_scale = scale(mat4(1), vec3(scaleFactor, scaleFactor, scaleFactor));
	//

	// Init Physics
	InitPhysicProperties(position, translation, rotation, degree, _scale, weight);
}

void RUnit::InitPhysicProperties(vec3 position, vec3 translation, vec3 rotation, float degree, mat4 scale, float weight)
{
	float numberOfVertices = 0;
	vec3 average(0, 0, 0);

	for (unsigned int i = 0; i < _model->meshes.size(); i++)
	{
		numberOfVertices += _model->meshes.at(i)._vertices.size();
	}

	// convex hull shape
	for (unsigned int i = 0; i < _model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < _model->meshes.at(i)._vertices.size(); j++)
		{
			vec3 newPos = scale * vec4(_model->meshes.at(i)._vertices.at(j).Position, 1.0f);
			shapeVector.push_back(newPos.x);
			shapeVector.push_back(newPos.y);
			shapeVector.push_back(newPos.z);

			average += _model->meshes.at(i)._vertices.at(j).Position * (1 / numberOfVertices);
		}
	}
	_middle = average;
	_shape = new btConvexHullShape(&shapeVector[0], shapeVector.size() / 3, 3 * sizeof(btScalar));
	//

	// radius of bounding sphere
	float farthest = 0.0f;
	for (unsigned int i = 0; i < _model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < _model->meshes.at(i)._vertices.size(); j++)
		{
			vec3 newPos = _model->meshes.at(i)._vertices.at(j).Position;
			float dist = distance(newPos, _middle);
			if (dist > farthest)
			{
				farthest = dist;
				mostDistant = _model->meshes.at(i)._vertices.at(j).Position;
			}
		}
	}
	//

	// Motion State
	btQuaternion rotationQuat;
	rotationQuat.setEulerZYX(rotation.x, rotation.y, rotation.z);
	btVector3 positionbT = btVector3(position.x, position.y, position.z);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotationQuat, positionbT));
	//

	// Weight
	btScalar mass = weight;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(mass, bodyInertia);
	//

	// Rigid Body
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, _shape, bodyInertia);
	bodyCI.m_restitution = 0.5f;
	bodyCI.m_friction = 0.005f;
	_body = new btRigidBody(bodyCI);
	//
	_collisionData = new CollisionData(_type);
	_collisionData->_parentRUnit = this;
	_body->setUserPointer(_collisionData);

	// Translation & Rotation
	_body->setLinearFactor(btVector3(1, 1, 1));
	_body->setLinearVelocity(btVector3(translation.x, translation.y, translation.z));
}

RUnit::RUnit()
{
	_model = nullptr;
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

long RUnit::draw(Frustum* frustum)
{
	unsigned int triangle = 0;
	if (_model != nullptr)
	{
		_model->setTransformMatrix(_transformation);
		vec4 middle = _transformation * vec4(_middle ,1);
		vec3 temp_dist = _transformation * vec4(mostDistant, 1);
		vec3 temp_middle = vec3(middle.x, middle.y, middle.z);

		float temp_radius = distance(temp_middle, temp_dist);

		if (frustum->Inside(temp_middle, temp_radius))
			triangle += _model->Draw();
	}
	for (int i = 0; i < this->children.size(); i++)
		triangle += this->children.at(i)->draw(frustum);
	return triangle;
}

void RUnit::addChild(RUnit* unit)
{
	this->children.push_back(unit);
}

void RUnit::update(mat4 transformation, float time)
{
	if (_model != nullptr)
	{
		btTransform transform = _body->getWorldTransform();
		btQuaternion rota = transform.getRotation();
		_translation = vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
		_rotation = vec3(rota.getX(), rota.getY(), rota.getZ());
		_degree = rota.getAngle();
		_transformation = translate(mat4(1), vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z())) * rotate(mat4(1), _degree, _rotation) * _scale;
	}

	for (int i = this->children.size() - 1; i >= 0; i--)
	{
		if (this->children.at(i)->_getDeleted)
		{
			this->children.erase(children.begin() + i);
		}
		else
		{
			this->children.at(i)->update(transformation, time);
		}
	}
}