#pragma once

#include "ShaderInterface.h"
#include "Texture.h"

class ShaderStencilTextureInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentUseTex;
	CGparameter fragmentTex;

public:
	ShaderStencilTextureInterface();

	bool InitProgram();
	void PerFrameInit();
};

