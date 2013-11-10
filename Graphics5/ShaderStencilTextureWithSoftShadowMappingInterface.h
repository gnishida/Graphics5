#pragma once

#include "ShaderInterface.h"

class ShaderStencilTextureWithSoftShadowMappingInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter fragmentMeshTris;
	CGparameter fragmentMeshVerts;
	CGparameter fragmentMeshTexCoords;
	CGparameter fragmentMeshTex;

	CGparameter fragmentLightOrig;
	CGparameter fragmentLightStep;
	CGparameter fragmentNumLight;

	CGparameter fragmentEyePosition;
	CGparameter fragmentKa;
	CGparameter fragmentKd;
	CGparameter fragmentKs;

	CGparameter fragmentUseTex;
	CGparameter fragmentTex;

	CGparameter fragmentModelViewProj;

public:
	ShaderStencilTextureWithSoftShadowMappingInterface();

	bool InitProgram();
	void PerFrameInit();
};

