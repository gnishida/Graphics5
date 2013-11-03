#pragma once

#include "ShaderInterface.h"
#include "M33.h"
#include "PPC.h"

class ShaderProjectiveTextureInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter vertexTextureMatrix;
	CGparameter fragmentProjectiveMap;

	PPC* ppc;
	M33 viewMat;

public:
	ShaderProjectiveTextureInterface();

	bool InitProgram();
	void PerFrameInit();

	void SetPPC(PPC* ppc);
	void BuildTextureMatrix(PPC* ppc, float textureMatrix[16]);
};

