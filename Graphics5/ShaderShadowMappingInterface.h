#pragma once

#include "ShaderInterface.h"
#include "M44.h"
#include "PPC.h"

class ShaderShadowMappingInterface : public ShaderInterface {
private:
	CGparameter vertexModelViewProj;
	CGparameter vertexTextureMatrix;
	CGparameter fragmentShadowMap;

	PPC* ppc;
	float zNear;
	float zFar;
	M33 viewMat;

public:
	ShaderShadowMappingInterface();

	bool InitProgram();
	void PerFrameInit();

	void SetPPC(PPC* ppc, float zNear, float zFar);

	void BuildBiasMatrix(M44& biasMatrix);
	void BuildLightViewMatrix(PPC* ppc, M44& lightViewMatrix);
	void BuildProjectionMatrix(PPC* ppc, float zNear, float zFar, M44& perspectiveMatrix);
};

