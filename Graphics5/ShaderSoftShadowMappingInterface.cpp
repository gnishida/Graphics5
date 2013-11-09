#include "ShaderSoftShadowMappingInterface.h"

ShaderSoftShadowMappingInterface::ShaderSoftShadowMappingInterface() {
}

bool ShaderSoftShadowMappingInterface::InitProgram() {
	if (!loadVertexProgram("CG/shaderSoftShadowMappingShading.cg", "VertexMain")) return false;

	if (!loadFragmentProgram("CG/shaderSoftShadowMappingShading.cg", "FragmentMain")) return false;

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");

	return true;
}

void ShaderSoftShadowMappingInterface::PerFrameInit() {
	// bind the shadow map
	GLuint texture_id = softShadowMap->depthBuffer->Bind();

	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	cgGLSetTextureParameter(fragmentShadowMap, texture_id);
}

void ShaderSoftShadowMappingInterface::SetSoftShadowMap(SoftShadowMap* softShadowMap) {
	this->softShadowMap = softShadowMap;
}