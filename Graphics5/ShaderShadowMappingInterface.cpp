#include "ShaderShadowMappingInterface.h"

ShaderShadowMappingInterface::ShaderShadowMappingInterface() {
}

bool ShaderShadowMappingInterface::InitProgram() {
	if (!loadVertexProgram("CG/shaderShadowMapping.cg", "VertexMain")) return false;

	if (!loadFragmentProgram("CG/shaderShadowMapping.cg", "FragmentMain")) return false;

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");
	vertexTextureMatrix = cgGetNamedParameter(vertexProgram, "textureMatrix");
	fragmentShadowMap = cgGetNamedParameter(fragmentProgram, "shadowMap");
	fragmentModelViewProj = cgGetNamedParameter(fragmentProgram, "modelViewProj");

	return true;
}

void ShaderShadowMappingInterface::PerFrameInit() {
	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	M44 biasMatrix, lightViewMatrix, projectionMatrix;
	BuildBiasMatrix(biasMatrix);
	BuildLightViewMatrix(ppc, lightViewMatrix);
	BuildProjectionMatrix(ppc, zNear, zFar, projectionMatrix);

	lightViewMatrix = biasMatrix * projectionMatrix * lightViewMatrix;
	cgSetMatrixParameterfr(vertexTextureMatrix, (float*)lightViewMatrix.rows);
	cgGLSetTextureParameter(fragmentShadowMap, 123);
	cgGLSetStateMatrixParameter(fragmentModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
}

