#pragma once

#include "ShaderInterface.h"
#include "M44.h"
#include "PPC.h"

class ShaderProjectorInterface : public ShaderInterface {
protected:
	PPC* ppc;
	float zNear;
	float zFar;
	M33 viewMat;

public:
	ShaderProjectorInterface();
	~ShaderProjectorInterface();

	void SetPPC(PPC* ppc, float zNear, float zFar);

	void BuildBiasMatrix(M44& biasMatrix);
	void BuildLightViewMatrix(PPC* ppc, M44& lightViewMatrix);
	void BuildProjectionMatrix(PPC* ppc, float zNear, float zFar, M44& perspectiveMatrix);
};

