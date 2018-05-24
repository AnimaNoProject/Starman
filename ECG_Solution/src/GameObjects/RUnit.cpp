#include "RUnit.h"

RUnit::RUnit(Model * model, vec3 translation, vec3 rotation, float degree, vec3 position, vec3 scaleIt)
{
	_model = model;
	_translation = translation;
	_rotation = rotation;
	_degree = degree;
	_position = translate(mat4(1), position);
	_scale = scale(mat4(1), scaleIt);

	//model->transform(_scale);


	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < model->meshes.at(i)._vertices.size(); j++)
		{
			shapeVector.push_back(model->meshes.at(i)._vertices.at(j).Position.x * scaleIt.x);
			shapeVector.push_back(model->meshes.at(i)._vertices.at(j).Position.y * scaleIt.x);
			shapeVector.push_back(model->meshes.at(i)._vertices.at(j).Position.z * scaleIt.x);
		}
	}

	_shape = new btConvexHullShape(&shapeVector[0], shapeVector.size() / 3, 3 * sizeof(btScalar));


	float numberOfVertices = 0;
	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		numberOfVertices += model->meshes.at(i)._vertices.size();
	}
	vec3 average(0, 0, 0);

	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < model->meshes.at(i)._vertices.size(); j++)
		{
			vec3 newPos = _scale * vec4(model->meshes.at(i)._vertices.at(j).Position, 1.0f);
			average.x += newPos.x;
			average.y += newPos.y;
			average.z += newPos.z;
		}
	}
	average.x /= numberOfVertices;
	average.y /= numberOfVertices;
	average.z /= numberOfVertices;
	bbmiddle = average;

	float farthest = 0.0f;

	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < model->meshes.at(i)._vertices.size(); j++)
		{
			vec3 newPos = _scale * vec4(model->meshes.at(i)._vertices.at(j).Position, 1.0f);
			float dist = distance(newPos, bbmiddle);

			if (dist > farthest)
			{
				farthest = dist;
			}
		}
	}

	radius = sqrt(farthest);

	btQuaternion rotationQuat;
	rotationQuat.setEulerZYX(rotation.x, rotation.y, rotation.z);
	btVector3 positionbT = btVector3(position.x, position.y, position.z);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotationQuat, positionbT));

	btScalar mass = 1500;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(mass, bodyInertia);

	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, _shape, bodyInertia);

	bodyCI.m_restitution = 0.5f;
	bodyCI.m_friction = 0.005f;

	_body = new btRigidBody(bodyCI);

	_body->setLinearFactor(btVector3(1, 1, 0));

	_body->setLinearVelocity(btVector3(translation.x, translation.y, translation.z));
}

RUnit::RUnit(Model * model)
{
	_model = model;
	float px, py, pz, r, tx, ty, tz, rx, ry, rz, s;
	tx = rand() & 10;
	ty = rand() & 10;
	tz = rand() & 10;
	/*
	px = rand() % 500;
	py = rand() % 500;
	pz = rand() % 500;
	*/
	px = rand() % 2;
	py = rand() % 2;
	pz = rand() % 2;

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

	//px = py = pz = 0;

	rx = rand() & 10;
	ry = rand() & 10;
	rz = rand() & 10;


	s = rand() % 25;
	r = rand() % 25 / 100;
	_translation = vec3(tx, ty, tz);
	_position = translate(mat4(1), vec3(px, py, pz));
	_rotation = vec3(rx, ry, rz);
	_scale = scale(mat4(1), vec3(s, s, s));
	_degree = r;
	_model = model;

	//model->transform(_scale);

	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < model->meshes.at(i)._vertices.size(); j++)
		{
			shapeVector.push_back(model->meshes.at(i)._vertices.at(j).Position.x * s);
			shapeVector.push_back(model->meshes.at(i)._vertices.at(j).Position.y * s);
			shapeVector.push_back(model->meshes.at(i)._vertices.at(j).Position.z * s);
		}
	}

	_shape = new btConvexHullShape(&shapeVector[0], shapeVector.size() / 3, 3 * sizeof(btScalar));

	float numberOfVertices = 0;
	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		numberOfVertices += model->meshes.at(i)._vertices.size();
	}
	vec3 average(0, 0, 0);

	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < model->meshes.at(i)._vertices.size(); j++)
		{
			vec3 newPos = _scale * vec4(model->meshes.at(i)._vertices.at(j).Position, 1.0f);
			average.x += newPos.x;
			average.y += newPos.y;
			average.z += newPos.z;
		}
	}
	average.x /= numberOfVertices;
	average.y /= numberOfVertices;
	average.z /= numberOfVertices;
	bbmiddle = average;

	float farthest = 0.0f;

	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		for (unsigned int j = 0; j < model->meshes.at(i)._vertices.size(); j++)
		{
			vec3 newPos = _scale * vec4(model->meshes.at(i)._vertices.at(j).Position, 1.0f);
			float dist = distance(newPos, bbmiddle);

			if (dist > farthest)
			{
				farthest = dist;
			}
		}
	}

	radius = sqrt(farthest);

	btQuaternion rotationQuat;
	rotationQuat.setEulerZYX(_rotation.x, _rotation.y, _rotation.z);
	btVector3 positionbT = btVector3(px, py, pz);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotationQuat, positionbT));

	btScalar mass = 1500;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(mass, bodyInertia);

	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, _shape, bodyInertia);

	bodyCI.m_restitution = 0.5f;
	bodyCI.m_friction = 0.005f;

	_body = new btRigidBody(bodyCI);

	_body->setLinearFactor(btVector3(1, 1, 0));

	_body->setLinearVelocity(btVector3(_translation.x, _translation.y, _translation.z));
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

long RUnit::draw(Frustum* frustum)
{
	unsigned int triangle = 0;
	if (_model != nullptr)
	{
		_model->setTransformMatrix(_transformation);

		
		//vec4 middle = vec4(bbmiddle, 1.0f);
		//middle = _transformation * middle;
		//if (frustum->Inside(middle, radius))
		//{
			triangle += _model->Draw();
		//}
	
	}
	for (int i = 0; i < this->children.size(); i++)
	{
		triangle += this->children.at(i)->draw(frustum);
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
		_translation = vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
		_rotation = vec3(rota.getX(), rota.getY(), rota.getZ());
		_degree = rota.getAngle();
		_transformation = translate(mat4(1), vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z())) * rotate(mat4(1), _degree, _rotation) * _scale;
	}

	for (int i = 0; i < this->children.size(); i++)
	{
		this->children.at(i)->update(transformation, time);
	}
}