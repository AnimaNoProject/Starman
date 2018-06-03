#pragma once
#include <string>

using namespace std;

class CollisionData
{
private:
	string _type;
	int _active;
	
public:
	CollisionData(string t, int a);
	~CollisionData();

	string getType();


};
