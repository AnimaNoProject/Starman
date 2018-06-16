#pragma once

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

class ProceduralTexture
{
private:
	std::vector<int> p;
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
public:
	ProceduralTexture();
	ProceduralTexture(unsigned int seed);
	double noise(double x, double y, double z);
	~ProceduralTexture();
};

