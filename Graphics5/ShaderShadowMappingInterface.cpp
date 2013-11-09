#include "ShaderShadowMappingInterface.h"
#include "TMesh.h"

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
	GLuint texture_id = shadowMap->getTexture()->Bind();

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

	cgGLSetTextureParameter(fragmentShadowMap, texture_id);
	cgGLSetStateMatrixParameter(fragmentModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
}

void ShaderShadowMappingInterface::SetShadowMap(ShadowMap* shadowMap, std::vector<TMesh*>* meshes) {
	this->shadowMap = shadowMap;

	// compute shadow map
	for (int i = 0; i < meshes->size(); i++) {
		meshes->at(i)->Render(shadowMap->fb, shadowMap->ppc);
	}
}

