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

	//float textureMatrix[16], perspectiveMatrix[16];
	M44 textureMatrix, perspectiveMatrix;
	BuildTextureMatrix(ppc, textureMatrix);
	BuildPerspectiveMatrix(ppc, zNear, zFar, perspectiveMatrix);

	textureMatrix = perspectiveMatrix * textureMatrix;
	cgSetMatrixParameterfr(vertexTextureMatrix, (float*)textureMatrix.rows);
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

void ShaderProjectiveTextureInterface::BuildTextureMatrix(PPC* ppc, M44& textureMatrix) {
	V3 z = (ppc->GetVD() * -1.0f).UnitVector();
	V3 y = ppc->b * -1.0f;
	V3 x = (y ^ z).UnitVector();
	y = (z ^ x).UnitVector();

	textureMatrix[0][0] = x.x();
	textureMatrix[0][1] = x.y();
	textureMatrix[0][2] = x.z();
	textureMatrix[0][3] = x * ppc->C * -1.0f;

	textureMatrix[0][0] = y.x();
	textureMatrix[0][1] = y.y();
	textureMatrix[0][2] = y.z();
	textureMatrix[0][3] = y * ppc->C * -1.0f;

	textureMatrix[0][0] = z.x();
	textureMatrix[0][1] = z.y();
	textureMatrix[0][2] = z.z();
	textureMatrix[0][3] = z * ppc->C * -1.0f;

	textureMatrix[0][0] = 0;
	textureMatrix[0][1] = 0;
	textureMatrix[0][2] = 0;
	textureMatrix[0][3] = 1;
}

void ShaderProjectiveTextureInterface::BuildPerspectiveMatrix(PPC* ppc, float zNear, float zFar, M44& perspectiveMatrix) {
	float aspectRatio = ppc->w / ppc->h;

	float theta = ppc->GetHFOV() / 2.0f * M_PI / 180.0f;

	perspectiveMatrix[0][0] = 1.0f / aspectRatio / tanf(theta);
	perspectiveMatrix[0][1] = 0;
	perspectiveMatrix[0][2] = 0;
	perspectiveMatrix[0][3] = 0;

	perspectiveMatrix[0][0] = 0;
	perspectiveMatrix[0][1] = 1.0f / tanf(theta);
	perspectiveMatrix[0][2] = 0;
	perspectiveMatrix[0][3] = 0;

	perspectiveMatrix[0][0] = 0;
	perspectiveMatrix[0][1] = zFar / (zFar - zNear);
	perspectiveMatrix[0][2] = zFar * zNear / (zFar - zNear);
	perspectiveMatrix[0][3] = 0;

	perspectiveMatrix[0][0] = 0;
	perspectiveMatrix[0][1] = 0;
	perspectiveMatrix[0][2] = 1;
	perspectiveMatrix[0][3] = 0;
}
