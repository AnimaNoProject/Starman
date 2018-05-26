#pragma once
#include <algorithm>

class Random
{
public:
	static int randomNumber(float min, float max) {
		return min + (rand() % static_cast<int>(max - min + 1));
	};
};