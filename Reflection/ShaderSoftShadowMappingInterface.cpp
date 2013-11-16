#include "ShaderSoftShadowMappingInterface.h"
#include "Scene.h"

ShaderSoftShadowMappingInterface::ShaderSoftShadowMappingInterface() {
}

bool ShaderSoftShadowMappingInterface::InitProgram() {
	//cgSetAutoCompile(cgContext, CG_COMPILE_MANUAL);

	if (!loadVertexProgram("CG/shaderSoftShadowMapping.cg", "VertexMain")) return false;

	if (!loadFragmentProgram("CG/shaderSoftShadowMapping.cg", "FragmentMain")) return false;

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");

	fragmentMeshTris = cgGetNamedParameter(fragmentProgram, "meshTris");
	fragmentMeshVerts = cgGetNamedParameter(fragmentProgram, "meshVerts");

	fragmentLightOrig = cgGetNamedParameter(fragmentProgram, "lightOrig");
	fragmentLightStep = cgGetNamedParameter(fragmentProgram, "lightStep");
	fragmentNumLight = cgGetNamedParameter(fragmentProgram, "numLight");

	fragmentEyePosition = cgGetNamedParameter(fragmentProgram, "eyePosition");
	fragmentKa = cgGetNamedParameter(fragmentProgram, "Ka");
	fragmentKd = cgGetNamedParameter(fragmentProgram, "Kd");
	fragmentKs = cgGetNamedParameter(fragmentProgram, "Ks");

	return true;
}

void ShaderSoftShadowMappingInterface::PerFrameInit() {
	// set parameters for the vertex shader
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	// copy the vertices data
	float tris[36 * 3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 36; j++) {
			tris[i * 36 + j] = (float)scene->meshes[i]->tris[j] + i * 24;
		}
	}
	float verts[24 * 3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 24; j++) {
			verts[i * 24 + j][0] = (float)scene->meshes[i]->verts[j].v.x();
			verts[i * 24 + j][1] = (float)scene->meshes[i]->verts[j].v.y();
			verts[i * 24 + j][2] = (float)scene->meshes[i]->verts[j].v.z();
		}
	}

	// set parameters for the fragment shader
	cgGLSetParameterArray1f(fragmentMeshTris, 0, 36 * 3, (float*)tris);
	cgGLSetParameterArray3f(fragmentMeshVerts, 0, 24 * 3, (float*)verts);

	cgGLSetParameter3fv(fragmentLightOrig, (float*)&scene->light->position);
	cgGLSetParameter1f(fragmentLightStep, 1.0f);
	cgGLSetParameter1f(fragmentNumLight, 1.0f);

	cgGLSetParameter3fv(fragmentEyePosition, (float*) &(scene->currentPPC->C));
	cgGLSetParameter3fv(fragmentKa, (float*) &(scene->light->ambient));
	cgGLSetParameter3fv(fragmentKd, (float*) &(scene->light->diffuse));
	cgGLSetParameter3fv(fragmentKs, (float*) &(scene->light->specular));

	//cgCompileProgram(fragmentProgram);
}
