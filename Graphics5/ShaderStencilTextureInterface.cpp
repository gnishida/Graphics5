#include "ShaderStencilTextureInterface.h"
#include "Scene.h"

ShaderStencilTextureInterface::ShaderStencilTextureInterface() {
}

bool ShaderStencilTextureInterface::InitProgram() {
	if (!loadVertexProgram("CG/shaderStencilTexture.cg", "VertexMain")) return false;

	if (!loadFragmentProgram("CG/shaderStencilTexture.cg", "FragmentMain")) return false;

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");

	fragmentLightPosition = cgGetNamedParameter(fragmentProgram, "lightPosition");
	fragmentEyePosition = cgGetNamedParameter(fragmentProgram, "eyePosition");
	fragmentKa = cgGetNamedParameter(fragmentProgram, "Ka");
	fragmentKd = cgGetNamedParameter(fragmentProgram, "Kd");
	fragmentKs = cgGetNamedParameter(fragmentProgram, "Ks");

	fragmentUseTex = cgGetNamedParameter(fragmentProgram, "useTex");
	fragmentTex = cgGetNamedParameter(fragmentProgram, "tex");

	fragmentModelViewProj = cgGetNamedParameter(fragmentProgram, "modelViewProj");

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

	cgGLSetParameter3fv(fragmentLightPosition, (float*) &(scene->light->position));
	cgGLSetParameter3fv(fragmentEyePosition, (float*) &(scene->currentPPC->C));
	cgGLSetParameter3fv(fragmentKa, (float*) &(scene->light->ambient));
	cgGLSetParameter3fv(fragmentKd, (float*) &(scene->light->diffuse));
	cgGLSetParameter3fv(fragmentKs, (float*) &(scene->light->specular));

	cgGLSetStateMatrixParameter(fragmentModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
}

