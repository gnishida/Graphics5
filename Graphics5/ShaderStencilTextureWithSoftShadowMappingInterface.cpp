#include "ShaderStencilTextureWithSoftShadowMappingInterface.h"
#include "Scene.h"

ShaderStencilTextureWithSoftShadowMappingInterface::ShaderStencilTextureWithSoftShadowMappingInterface() {
}

bool ShaderStencilTextureWithSoftShadowMappingInterface::InitProgram() {
	//cgSetAutoCompile(cgContext, CG_COMPILE_MANUAL);

	if (!loadVertexProgram("CG/shaderStencilTextureWithSoftShadowMapping.cg", "VertexMain")) return false;

	if (!loadFragmentProgram("CG/shaderStencilTextureWithSoftShadowMapping.cg", "FragmentMain")) return false;

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");

	fragmentMeshTris = cgGetNamedParameter(fragmentProgram, "meshTris");
	fragmentMeshVerts = cgGetNamedParameter(fragmentProgram, "meshVerts");
	fragmentMeshNorms = cgGetNamedParameter(fragmentProgram, "meshNorms");
	fragmentMeshTexCoords = cgGetNamedParameter(fragmentProgram, "meshTexCoords");

	fragmentLightCenter = cgGetNamedParameter(fragmentProgram, "lightCenter");
	fragmentLightStep = cgGetNamedParameter(fragmentProgram, "lightStep");
	fragmentNumLight = cgGetNamedParameter(fragmentProgram, "numLight");

	fragmentEyePosition = cgGetNamedParameter(fragmentProgram, "eyePosition");
	fragmentKa = cgGetNamedParameter(fragmentProgram, "Ka");
	fragmentKd = cgGetNamedParameter(fragmentProgram, "Kd");
	fragmentKs = cgGetNamedParameter(fragmentProgram, "Ks");

	fragmentUseTex = cgGetNamedParameter(fragmentProgram, "useTex");
	fragmentTex = cgGetNamedParameter(fragmentProgram, "tex");

	fragmentModelViewProj = cgGetNamedParameter(fragmentProgram, "modelViewProj");

	return true;
}

void ShaderStencilTextureWithSoftShadowMappingInterface::PerFrameInit() {
	// set parameters for the vertex shader
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	// bind the texture
	if (mesh->texture != NULL) {
		GLuint texture_id = mesh->texture->Bind();
		cgGLSetTextureParameter(fragmentTex, texture_id);
		cgGLSetParameter1f(fragmentUseTex, 1.0f);
	} else {
		cgGLSetParameter1f(fragmentUseTex, 0.0f);
	}

	// copy the vertices data
	float tris[3][18];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			tris[i][j * 3] = (float)scene->meshes[i]->tris[j * 6] + i * 24;
			tris[i][j * 3 + 1] = (float)scene->meshes[i]->tris[j * 6 + 1] + i * 24;
			tris[i][j * 3 + 2] = (float)scene->meshes[i]->tris[j * 6 + 5] + i * 24;
		}
	}

	float verts[3][24][3];
	float norms[3][24][3];
	float texCoords[3][24][2];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 24; j++) {
			verts[i][j][0] = (float)scene->meshes[i]->verts[j].v.x();
			verts[i][j][1] = (float)scene->meshes[i]->verts[j].v.y();
			verts[i][j][2] = (float)scene->meshes[i]->verts[j].v.z();
			norms[i][j][0] = (float)scene->meshes[i]->verts[j].n.x();
			norms[i][j][1] = (float)scene->meshes[i]->verts[j].n.y();
			norms[i][j][2] = (float)scene->meshes[i]->verts[j].n.z();
			texCoords[i][j][0] = (float)scene->meshes[i]->verts[j].t[0];
			texCoords[i][j][1] = (float)scene->meshes[i]->verts[j].t[1];
		}
	}

	// set parameters for the fragment shader
	cgGLSetParameterArray1f(fragmentMeshTris, 0, 18 * 3, (float*)tris[0]);
	cgGLSetParameterArray3f(fragmentMeshVerts, 0, 24 * 3, (float*)verts[0]);
	cgGLSetParameterArray3f(fragmentMeshNorms, 0, 24 * 3, (float*)norms[0]);
	cgGLSetParameterArray2f(fragmentMeshTexCoords, 0, 24 * 3, (float*)texCoords[0]);

	cgGLSetParameter3fv(fragmentLightCenter, (float*)&scene->light->position);
	cgGLSetParameter1f(fragmentLightStep, (float)scene->light_step);
	cgGLSetParameter1f(fragmentNumLight, 1.0f);

	cgGLSetParameter3fv(fragmentEyePosition, (float*) &(scene->currentPPC->C));
	cgGLSetParameter3fv(fragmentKa, (float*) &(scene->light->ambient));
	cgGLSetParameter3fv(fragmentKd, (float*) &(scene->light->diffuse));
	cgGLSetParameter3fv(fragmentKs, (float*) &(scene->light->specular));

	cgGLSetStateMatrixParameter(fragmentModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	//cgCompileProgram(fragmentProgram);
}
