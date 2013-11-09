#include "SoftShadowMap.h"

SoftShadowMap::SoftShadowMap(const V3& lightCenter, const V3& dir, const float length, int numLight) {
	this->dir = dir.UnitVector();

	V3 y(0.0, 1.0f, 0.0f);
	V3 x = (dir ^ y).UnitVector();
	y = (x ^ dir).UnitVector();

	float step = length / (float)(numLight - 1);

	// define the location of each light source
	V3 bottomLeft = lightCenter - x * length * 0.5f - y * length * 0.5f;
	for (int i = 0; i < numLight; i++) {
		for (int j = 0; j < numLight; j++) {
			V3 p = bottomLeft + x * j * step + y * i * step;
			lights.push_back(p);
		}
	}
}
