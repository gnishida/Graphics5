#pragma once

#include "ShaderInterface.h"

class ShaderTextureInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentTex;

public:
	ShaderTextureInterface();

	bool InitProgram();
	void BindPrograms();
	void PerFrameInit();
	void PerFrameDisable();
};

