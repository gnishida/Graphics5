#pragma once

#include "ShaderInterface.h"

class ShaderSoftShadowMappingInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentMeshTris;
	CGparameter fragmentMeshVerts;

	CGparameter fragmentLightOrig;
	CGparameter fragmentLightStep;
	CGparameter fragmentNumLight;

	CGparameter fragmentEyePosition;
	CGparameter fragmentKa;
	CGparameter fragmentKd;
	CGparameter fragmentKs;

public:
	ShaderSoftShadowMappingInterface();

	bool InitProgram();
	void PerFrameInit();
};

