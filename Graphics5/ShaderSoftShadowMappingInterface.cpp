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

	fragmentBox1Tris = cgGetNamedParameter(fragmentProgram, "box1Tris");
	fragmentBox1Verts = cgGetNamedParameter(fragmentProgram, "box1Verts");

	/*
	fragmentBox2Tris = cgGetNamedParameter(fragmentProgram, "box2Tris");
	fragmentBox2Verts = cgGetNamedParameter(fragmentProgram, "box2Verts");

	fragmentBox3Tris = cgGetNamedParameter(fragmentProgram, "box3Tris");
	fragmentBox3Verts = cgGetNamedParameter(fragmentProgram, "box3Verts");
	*/

	fragmentLightOrig = cgGetNamedParameter(fragmentProgram, "lightOrig");
	fragmentLightStep = cgGetNamedParameter(fragmentProgram, "lightStep");
	fragmentNumLight = cgGetNamedParameter(fragmentProgram, "numLight");

	return true;
}

void ShaderSoftShadowMappingInterface::PerFrameInit() {
	// set parameters for the vertex shader
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	// copy the vertices data
	float tris[3][36];
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 36; j++) {
			tris[i][j] = (float)scene->meshes[i]->tris[j];
		}
	}
	float verts[3][24][3];
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 24; j++) {
			verts[i][j][0] = (float)scene->meshes[i]->verts[j].v.x();
			verts[i][j][1] = (float)scene->meshes[i]->verts[j].v.y();
			verts[i][j][2] = (float)scene->meshes[i]->verts[j].v.z();
		}
	}

	float lightOrig[3] = {-40.0f, 40.0f, -20.0f};

	// set parameters for the fragment shader
	cgGLSetParameterArray1f(fragmentBox1Tris, 0, 36, (float*)tris[0]);
	cgGLSetParameterArray3f(fragmentBox1Verts, 0, 24, (float*)verts[0]);

	/*
	cgGLSetParameterArray1f(fragmentBox2Tris, 0, 36, (float*)tris[1]);
	cgGLGetParameterArray3f(fragmentBox2Verts, 0, 8, (float*)verts[1]);

	cgGLSetParameterArray1f(fragmentBox3Tris, 0, 36, (float*)tris[2]);
	cgGLSetParameterArray3f(fragmentBox3Verts, 0, 8, (float*)verts[2]);
	*/

	cgGLSetParameter3fv(fragmentLightOrig, (float*)&scene->light->position);
	cgGLSetParameter1f(fragmentLightStep, 1.0f);
	cgGLSetParameter1f(fragmentNumLight, 16.0f);

	//cgCompileProgram(fragmentProgram);
}
