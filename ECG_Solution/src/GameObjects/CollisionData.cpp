#include "CollisionData.h"


CollisionData::CollisionData(std::string t)
{
	_type = t;
}

CollisionData::~CollisionData()
{
}


std::string CollisionData::getType()
{
	return _type;
}


void CollisionData::setParentRUnit(RUnit* _p)
{
	_parentRUnit = _p;
}



