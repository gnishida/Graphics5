#pragma once

#include "ShaderProjectorInterface.h"
#include "M44.h"
#include "PPC.h"

class ShaderProjectiveTextureInterface : public ShaderProjectorInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter vertexTextureMatrix;
	CGparameter fragmentProjectiveMap;

public:
	ShaderProjectiveTextureInterface();

	bool InitProgram();
	void PerFrameInit();

	void SetPPC(PPC* ppc, float zNear, float zFar);

	
	
};

