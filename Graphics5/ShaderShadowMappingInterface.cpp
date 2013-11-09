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
	// bind shadow mapping buffer
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	float zNear = 1.0f;
	float zFar = 1000.0f;
	M44 biasMatrix, lightViewMatrix, projectionMatrix;
	BuildBiasMatrix(biasMatrix);
	BuildLightViewMatrix(shadowMap->ppc, lightViewMatrix);
	BuildProjectionMatrix(shadowMap->ppc, zNear, zFar, projectionMatrix);

	lightViewMatrix = biasMatrix * projectionMatrix * lightViewMatrix;
	cgSetMatrixParameterfr(vertexTextureMatrix, (float*)lightViewMatrix.rows);

	cgGLSetTextureParameter(fragmentShadowMap, depthTexture);
	cgGLSetStateMatrixParameter(fragmentModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
}

void ShaderShadowMappingInterface::SetShadowMap(ShadowMap* shadowMap) {
	this->shadowMap = shadowMap;
}