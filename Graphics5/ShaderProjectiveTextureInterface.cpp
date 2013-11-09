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
