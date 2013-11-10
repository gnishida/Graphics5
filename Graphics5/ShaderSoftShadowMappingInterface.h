#pragma once

#include "ShaderInterface.h"
#include "SoftShadowMap.h"

class ShaderSoftShadowMappingInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentMeshTris;
	CGparameter fragmentMeshVerts;

	CGparameter fragmentLightOrig;
	CGparameter fragmentLightStep;
	CGparameter fragmentNumLight;

public:
	ShaderSoftShadowMappingInterface();

	bool InitProgram();
	void PerFrameInit();
};

