#include "ShaderProjectiveTextureInterface.h"

ShaderProjectiveTextureInterface::ShaderProjectiveTextureInterface() {
	ppc = NULL;
}

bool ShaderProjectiveTextureInterface::InitProgram() {
	if (!loadVertexProgram("CG/shaderProjectiveTexture.cg", "VertexMain")) return false;

	if (!loadFragmentProgram("CG/shaderProjectiveTexture.cg", "FragmentMain")) return false;

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");
	vertexTextureMatrix = cgGetNamedParameter(vertexProgram, "textureMatrix");
	fragmentProjectiveMap = cgGetNamedParameter(fragmentProgram, "projectiveMap");

	return true;
}

void ShaderProjectiveTextureInterface::PerFrameInit() {
	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	M44 biasMatrix, lightViewMatrix, projectionMatrix;
	BuildBiasMatrix(biasMatrix);
	BuildLightViewMatrix(ppc, lightViewMatrix);
	BuildProjectionMatrix(ppc, zNear, zFar, projectionMatrix);

	lightViewMatrix = biasMatrix * projectionMatrix * lightViewMatrix;
	cgSetMatrixParameterfr(vertexTextureMatrix, (float*)lightViewMatrix.rows);
	cgGLSetTextureParameter(fragmentProjectiveMap, 123);
}

/**
 * Set camera which is used for the projective texture mapping.
 */
void ShaderProjectiveTextureInterface::SetPPC(PPC* ppc, float zNear, float zFar) {
	this->ppc = ppc;
	this->zNear = zNear;
	this->zFar = zFar;
}

void ShaderProjectiveTextureInterface::BuildBiasMatrix(M44& biasMatrix) {
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

void ShaderProjectiveTextureInterface::BuildLightViewMatrix(PPC* ppc, M44& lightViewMatrix) {
	V3 z = (ppc->GetVD() * -1.0f).UnitVector();
	V3 y = ppc->b;
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

void ShaderProjectiveTextureInterface::BuildProjectionMatrix(PPC* ppc, float zNear, float zFar, M44& perspectiveMatrix) {
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
