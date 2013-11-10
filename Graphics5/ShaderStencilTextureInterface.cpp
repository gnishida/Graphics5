#include "ShaderStencilTextureInterface.h"

ShaderStencilTextureInterface::ShaderStencilTextureInterface() {
}

bool ShaderStencilTextureInterface::InitProgram() {
	if (!loadVertexProgram("CG/shaderStencilTexture.cg", "VertexMain")) return false;

	if (!loadFragmentProgram("CG/shaderStencilTexture.cg", "FragmentMain")) return false;

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");
	fragmentUseTex = cgGetNamedParameter(fragmentProgram, "useTex");
	fragmentTex = cgGetNamedParameter(fragmentProgram, "tex");

	return true;
}


void ShaderStencilTextureInterface::PerFrameInit() {
	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	if (mesh->texture != NULL) {
		GLuint texture_id = mesh->texture->Bind();
		cgGLSetTextureParameter(fragmentTex, texture_id);
		cgGLSetParameter1f(fragmentUseTex, 1.0f);
	} else {
		cgGLSetParameter1f(fragmentUseTex, 0.0f);
	}
}

