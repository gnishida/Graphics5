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
	/*
	float tris[3][15];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			tris[i][j * 3] = (float)scene->meshes[i]->tris[(j + 1) * 6] + i * 24;
			tris[i][j * 3 + 1] = (float)scene->meshes[i]->tris[(j + 1) * 6 + 1] + i * 24;
			tris[i][j * 3 + 2] = (float)scene->meshes[i]->tris[(j + 1) * 6 + 5] + i * 24;
		}
	}
	*/

	float verts[3][15][3];
	float norms[3][5][3];
	float texCoords[3][15][2];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			verts[i][j * 3][0] = (float)scene->meshes[i]->verts[(j + 1) * 4].v.x();
			verts[i][j * 3][1] = (float)scene->meshes[i]->verts[(j + 1) * 4].v.y();
			verts[i][j * 3][2] = (float)scene->meshes[i]->verts[(j + 1) * 4].v.z();

			verts[i][j * 3 + 1][0] = (float)scene->meshes[i]->verts[(j + 1) * 4 + 1].v.x();
			verts[i][j * 3 + 1][1] = (float)scene->meshes[i]->verts[(j + 1) * 4 + 1].v.y();
			verts[i][j * 3 + 1][2] = (float)scene->meshes[i]->verts[(j + 1) * 4 + 1].v.z();

			verts[i][j * 3 + 2][0] = (float)scene->meshes[i]->verts[(j + 1) * 4 + 3].v.x();
			verts[i][j * 3 + 2][1] = (float)scene->meshes[i]->verts[(j + 1) * 4 + 3].v.y();
			verts[i][j * 3 + 2][2] = (float)scene->meshes[i]->verts[(j + 1) * 4 + 3].v.z();

			norms[i][j][0] = (float)scene->meshes[i]->verts[(j + 1) * 4].n.x();
			norms[i][j][1] = (float)scene->meshes[i]->verts[(j + 1) * 4].n.y();
			norms[i][j][2] = (float)scene->meshes[i]->verts[(j + 1) * 4].n.z();

			texCoords[i][j * 3][0] = (float)scene->meshes[i]->verts[(j + 1) * 4].t[0];
			texCoords[i][j * 3][1] = (float)scene->meshes[i]->verts[(j + 1) * 4].t[1];

			texCoords[i][j * 3 + 1][0] = (float)scene->meshes[i]->verts[(j + 1) * 4 + 1].t[0];
			texCoords[i][j * 3 + 1][1] = (float)scene->meshes[i]->verts[(j + 1) * 4 + 1].t[1];

			texCoords[i][j * 3 + 2][0] = (float)scene->meshes[i]->verts[(j + 1) * 4 + 3].t[0];
			texCoords[i][j * 3 + 2][1] = (float)scene->meshes[i]->verts[(j + 1) * 4 + 3].t[1];
		}
	}

	// set parameters for the fragment shader
	//cgGLSetParameterArray1f(fragmentMeshTris, 0, 15 * 3, (float*)tris[0]);
	cgGLSetParameterArray3f(fragmentMeshVerts, 0, 15 * 3, (float*)verts[0]);
	cgGLSetParameterArray3f(fragmentMeshNorms, 0, 5 * 3, (float*)norms[0]);
	cgGLSetParameterArray2f(fragmentMeshTexCoords, 0, 15 * 3, (float*)texCoords[0]);

	cgGLSetParameter3fv(fragmentLightCenter, (float*)&scene->light->position);
	cgGLSetParameter1f(fragmentLightStep, (float)scene->light_step);
	cgGLSetParameter1f(fragmentNumLight, 5.0f);

	cgGLSetParameter3fv(fragmentEyePosition, (float*) &(scene->currentPPC->C));
	cgGLSetParameter3fv(fragmentKa, (float*) &(scene->light->ambient));
	cgGLSetParameter3fv(fragmentKd, (float*) &(scene->light->diffuse));
	cgGLSetParameter3fv(fragmentKs, (float*) &(scene->light->specular));

	cgGLSetStateMatrixParameter(fragmentModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	//cgCompileProgram(fragmentProgram);
}
