#pragma once

#include "ShaderInterface.h"
#include "SoftShadowMap.h"

class ShaderSoftShadowMappingInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentShadowMap;

	SoftShadowMap* softShadowMap;

public:
	ShaderSoftShadowMappingInterface();

	bool InitProgram();
	void PerFrameInit();

	void SetSoftShadowMap(SoftShadowMap* softShadowMap);
};

