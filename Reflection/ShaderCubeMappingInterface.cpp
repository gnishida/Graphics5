#include "ShaderCubeMappingInterface.h"
#include "Scene.h"

ShaderCubeMappingInterface::ShaderCubeMappingInterface() {
}

bool ShaderCubeMappingInterface::InitProgram() {
	if (!loadVertexProgram("CG/shaderCubeMapping.cg", "VertexMain")) return false;

	if (!loadFragmentProgram("CG/shaderCubeMapping.cg", "FragmentMain")) return false;

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");
	fragmentEyePosition = cgGetNamedParameter(fragmentProgram, "eyePosition");
	fragmentEnvironmentMap = cgGetNamedParameter(fragmentProgram, "environmentMap");

	return true;
}

void ShaderCubeMappingInterface::PerFrameInit() {
	GLuint cubemap_id = mesh->cubeMap->Bind();

	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	cgGLSetParameter3fv(fragmentEyePosition, (float*) &(scene->currentPPC->C));
	cgGLSetTextureParameter(fragmentEnvironmentMap, cubemap_id);
}
