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

	float textureMatrix[16];
	BuildTextureMatrix(ppc, textureMatrix);

	cgSetMatrixParameterfr(vertexTextureMatrix, (float*)textureMatrix);
	cgGLSetTextureParameter(fragmentProjectiveMap, 123);
}

/**
 * Set camera which is used for the projective texture mapping.
 */
void ShaderProjectiveTextureInterface::SetPPC(PPC* ppc) {
	this->ppc = ppc;
}

void ShaderProjectiveTextureInterface::BuildTextureMatrix(PPC* ppc, float textureMatrix[16]) {
	V3 z = (ppc->GetVD() * -1.0f).UnitVector();
	V3 y = ppc->b * -1.0f;
	V3 x = (y ^ z).UnitVector();
	y = (z ^ x).UnitVector();

	textureMatrix[0] = x.x();
	textureMatrix[1] = x.y();
	textureMatrix[2] = x.z();
	textureMatrix[3] = x * ppc->C * -1.0f;

	textureMatrix[4] = y.x();
	textureMatrix[5] = y.y();
	textureMatrix[6] = y.z();
	textureMatrix[7] = y * ppc->C * -1.0f;

	textureMatrix[8] = z.x();
	textureMatrix[9] = z.y();
	textureMatrix[10] = z.z();
	textureMatrix[11] = z * ppc->C * -1.0f;

	textureMatrix[12] = 0;
	textureMatrix[13] = 0;
	textureMatrix[14] = 0;
	textureMatrix[15] = 1;

}
