#pragma once

#include "ShaderInterface.h"
#include "Texture.h"

class ShaderStencilTextureInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentTex;

	Texture* texture;

public:
	ShaderStencilTextureInterface();

	bool InitProgram();
	void PerFrameInit();

	void SetTexture(Texture* texture);
};

