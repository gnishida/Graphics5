#include "ShaderProjectorInterface.h"

ShaderProjectorInterface::ShaderProjectorInterface() {
}

ShaderProjectorInterface::~ShaderProjectorInterface() {
}

/**
 * Set camera which is used for the projective texture mapping.
 */
void ShaderProjectorInterface::SetPPC(PPC* ppc, float zNear, float zFar) {
	this->ppc = ppc;
	this->zNear = zNear;
	this->zFar = zFar;
}

/**
 * Build a bias matrix which is used to transform the coordinate [-1,1] to the texture coordinate [0,1].
 *
 * @biasMatrix		the results 
 */
void ShaderProjectorInterface::BuildBiasMatrix(M44& biasMatrix) {
	biasMatrix[0][0] = 0.5f;
	biasMatrix[0][1] = 0.0f;
	biasMatrix[0][2] = 0.0f;
	biasMatrix[0][3] = 0.5f;

	biasMatrix[1][0] = 0.0f;
	biasMatrix[1][1] = 0.5f;
	biasMatrix[1][2] = 0.0f;
	biasMatrix[1][3] = 0.5f;

	biasMatrix[2][0] = 0.0f;
	biasMatrix[2][1] = 0.0f;
	biasMatrix[2][2] = 0.5f;
	biasMatrix[2][3] = 0.5f;

	biasMatrix[3][0] = 0.0f;
	biasMatrix[3][1] = 0.0f;
	biasMatrix[3][2] = 0.0f;
	biasMatrix[3][3] = 1.0f;
}

/**
 * Build a light view matrix
 */
void ShaderProjectorInterface::BuildLightViewMatrix(PPC* ppc, M44& lightViewMatrix) {
	V3 z = (ppc->GetVD() * -1.0f).UnitVector();
	V3 y = ppc->b * -1.0f;
	//V3 y(0.0f, -1.0f, 0.0f);
	V3 x = (y ^ z).UnitVector();
	y = (z ^ x).UnitVector();

	lightViewMatrix[0][0] = x.x();
	lightViewMatrix[0][1] = x.y();
	lightViewMatrix[0][2] = x.z();
	lightViewMatrix[0][3] = x * ppc->C * -1.0f;

	lightViewMatrix[1][0] = y.x();
	lightViewMatrix[1][1] = y.y();
	lightViewMatrix[1][2] = y.z();
	lightViewMatrix[1][3] = y * ppc->C * -1.0f;

	lightViewMatrix[2][0] = z.x();
	lightViewMatrix[2][1] = z.y();
	lightViewMatrix[2][2] = z.z();
	lightViewMatrix[2][3] = z * ppc->C * -1.0f;

	lightViewMatrix[3][0] = 0;
	lightViewMatrix[3][1] = 0;
	lightViewMatrix[3][2] = 0;
	lightViewMatrix[3][3] = 1;
}

/**
 * Build a projection matrix.
 *
 */
void ShaderProjectorInterface::BuildProjectionMatrix(PPC* ppc, float zNear, float zFar, M44& perspectiveMatrix) {
	float aspectRatio = ppc->w / ppc->h;

	float theta = ppc->GetHFOV() / 2.0f * M_PI / 180.0f;

	perspectiveMatrix[0][0] = 1.0f / aspectRatio / tanf(theta);
	perspectiveMatrix[0][1] = 0;
	perspectiveMatrix[0][2] = 0;
	perspectiveMatrix[0][3] = 0;

	perspectiveMatrix[1][0] = 0;
	perspectiveMatrix[1][1] = 1.0f / tanf(theta);
	perspectiveMatrix[1][2] = 0;
	perspectiveMatrix[1][3] = 0;

	perspectiveMatrix[2][0] = 0;
	perspectiveMatrix[2][1] = 0;
	perspectiveMatrix[2][2] = -(zFar + zNear) / (zFar - zNear);
	perspectiveMatrix[2][3] = -2 * zNear * zFar / (zFar - zNear);

	perspectiveMatrix[3][0] = 0;
	perspectiveMatrix[3][1] = 0;
	perspectiveMatrix[3][2] = -1;
	perspectiveMatrix[3][3] = 0;
}
