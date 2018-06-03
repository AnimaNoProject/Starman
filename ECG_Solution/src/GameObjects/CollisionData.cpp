#include "CollisionData.h"

CollisionData::CollisionData(string t, int a)
{
	_type = t;
	_active = a;
}

CollisionData::~CollisionData()
{

}


string CollisionData::getType()
{
	return _type;
}

