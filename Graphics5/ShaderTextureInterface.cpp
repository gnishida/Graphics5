#include "ShaderTextureInterface.h"
#include "Scene.h"

ShaderTextureInterface::ShaderTextureInterface(void) {
}

bool ShaderTextureInterface::InitProgram() {
	if (!loadVertexProgram("CG/shaderTexture.cg", "VertexMain")) return false;

	if (!loadFragmentProgram("CG/shaderTexture.cg", "FragmentMain")) return false;

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");
	fragmentTex = cgGetNamedParameter(fragmentProgram, "tex");

	return true;
}


void ShaderTextureInterface::PerFrameInit() {
	GLuint texture_id = texture->Bind();

	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetTextureParameter(fragmentTex, texture_id);
}

void ShaderTextureInterface::SetTexture(Texture* texture) {
	this->texture = texture;
}
