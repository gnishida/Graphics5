#pragma once

#include "ShaderInterface.h"

class ShaderReflectionInterface : public ShaderInterface{
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentLightDirection;
	CGparameter fragmentEyePosition;
	CGparameter fragmentKa;
	CGparameter fragmentKd;
	CGparameter fragmentKs;

public:
	ShaderReflectionInterface();

	bool InitProgram();
	void PerFrameInit();
};

