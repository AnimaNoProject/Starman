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


void CollisionData::setParentRUnit(RUnit* unit)
{
	_parentRUnit = unit;
}

void CollisionData::setParentShot(Shots* shot)
{
	_parentShot = shot;
}

void CollisionData::setParentPlayer(RPlayer* player)
{
	_parentPlayer = player;
}



