#pragma once

#include "ShaderInterface.h"

class ShaderCubeMappingInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentEyePosition;
	CGparameter fragmentEnvironmentMap;

public:
	ShaderCubeMappingInterface();

	bool InitProgram();
	void PerFrameInit();
};

