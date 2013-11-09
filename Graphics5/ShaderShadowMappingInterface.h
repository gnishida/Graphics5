#pragma once

#include "ShaderProjectorInterface.h"
#include "M44.h"
#include "PPC.h"
#include "ShadowMap.h"

class ShaderShadowMappingInterface : public ShaderProjectorInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter vertexTextureMatrix;
	CGparameter fragmentShadowMap;
	CGparameter fragmentModelViewProj;

	ShadowMap* shadowMap;

public:
	ShaderShadowMappingInterface();

	bool InitProgram();
	void PerFrameInit();

	void SetShadowMap(ShadowMap* shadowMap);
};

