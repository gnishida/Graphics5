#include "ShaderReflectionInterface.h"
#include "Scene.h"

ShaderReflectionInterface::ShaderReflectionInterface() {
}

bool ShaderReflectionInterface::InitProgram() {
	if (!loadVertexProgram("CG/shaderReflection.cg", "VertexMain")) return false;

	if (!loadFragmentProgram("CG/shaderReflection.cg", "FragmentMain")) return false;

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");
	fragmentLightDirection = cgGetNamedParameter(fragmentProgram, "lightDirection");
	fragmentEyePosition = cgGetNamedParameter(fragmentProgram, "eyePosition");
	fragmentKa = cgGetNamedParameter(fragmentProgram, "Ka");
	fragmentKd = cgGetNamedParameter(fragmentProgram, "Kd");
	fragmentKs = cgGetNamedParameter(fragmentProgram, "Ks");

	return true;
}

void ShaderReflectionInterface::PerFrameInit() {
	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	cgGLSetParameter3fv(fragmentLightDirection, (float*) &(scene->light->position));
	cgGLSetParameter3fv(fragmentEyePosition, (float*) &(scene->currentPPC->C));
	cgGLSetParameter3fv(fragmentKa, (float*) &(scene->light->ambient));
	cgGLSetParameter3fv(fragmentKd, (float*) &(scene->light->diffuse));
	cgGLSetParameter3fv(fragmentKs, (float*) &(scene->light->specular));
}