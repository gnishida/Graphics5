#pragma once

#include "ShaderInterface.h"
#include "Texture.h"

class ShaderTextureInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentTex;

public:
	ShaderTextureInterface();

	bool InitProgram();
	void PerFrameInit();
};

