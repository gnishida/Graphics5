#pragma once

#include "ShaderInterface.h"

class ShaderPhongShadingInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentLightDirection;
	CGparameter fragmentEyePosition;
	CGparameter fragmentKa;
	CGparameter fragmentKd;
	CGparameter fragmentKs;

public:
	ShaderPhongShadingInterface();

	bool InitProgram();
	void BindPrograms();
	void PerFrameInit();
	void PerFrameDisable();
};

