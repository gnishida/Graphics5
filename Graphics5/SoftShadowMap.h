#pragma once

#include "V3.h"
#include "Texture.h"
#include <vector>

class SoftShadowMap {
public:
	V3 dir;
	std::vector<V3> lights;
	Texture* depthBuffer;

public:
	SoftShadowMap(const V3& lightCenter, const V3& dir, const float length, int numLight);
};

