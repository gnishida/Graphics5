#pragma once

#include "ShaderInterface.h"
#include "SoftShadowMap.h"

class ShaderSoftShadowMappingInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentBox1Tris;
	CGparameter fragmentBox1Verts;

	CGparameter fragmentBox2Tris;
	CGparameter fragmentBox2Verts;

	CGparameter fragmentBox3Tris;
	CGparameter fragmentBox3Verts;

	CGparameter fragmentLightOrig;
	CGparameter fragmentLightStep;
	CGparameter fragmentNumLight;

public:
	ShaderSoftShadowMappingInterface();

	bool InitProgram();
	void PerFrameInit();
};

