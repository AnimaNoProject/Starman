#include "CollisionData.h"
//#include "Shots.h"
//#include "RUnit.h"


CollisionData::CollisionData(string t)
{
	_type = t;
}

CollisionData::~CollisionData()
{
}


string CollisionData::getType()
{
	return _type;
}


void CollisionData::setParentRUnit(RUnit* _p)
{
	_parentRUnit = _p;
}



