#pragma once

#include "ShaderInterface.h"
#include "Texture.h"

class ShaderStencilTextureInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;

	CGparameter fragmentLightPosition;
	CGparameter fragmentEyePosition;
	CGparameter fragmentKa;
	CGparameter fragmentKd;
	CGparameter fragmentKs;
	CGparameter fragmentUseTex;
	CGparameter fragmentTex;
	CGparameter fragmentModelViewProj;

public:
	ShaderStencilTextureInterface();

	bool InitProgram();
	void PerFrameInit();
};

