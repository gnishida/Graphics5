#pragma once

#include "ShaderInterface.h"
#include "M44.h"
#include "PPC.h"

class ShaderProjectiveTextureInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter vertexTextureMatrix;
	CGparameter fragmentProjectiveMap;

	PPC* ppc;
	float zNear;
	float zFar;
	M33 viewMat;

public:
	ShaderProjectiveTextureInterface();

	bool InitProgram();
	void PerFrameInit();

	void SetPPC(PPC* ppc, float zNear, float zFar);

	void BuildBiasMatrix(M44& biasMatrix);
	void BuildLightViewMatrix(PPC* ppc, M44& lightViewMatrix);
	void BuildProjectionMatrix(PPC* ppc, float zNear, float zFar, M44& perspectiveMatrix);
};

