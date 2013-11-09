#pragma once

#include "ShaderProjectorInterface.h"
#include "M44.h"
#include "PPC.h"

class ShaderShadowMappingInterface : public ShaderProjectorInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter vertexTextureMatrix;
	CGparameter fragmentShadowMap;
	CGparameter fragmentModelViewProj;

public:
	ShaderShadowMappingInterface();

	bool InitProgram();
	void PerFrameInit();
};

