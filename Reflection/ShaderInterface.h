#pragma once

#include "TMesh.h"
#include <Cg/cgGL.h>
#include <Cg/cg.h>

class ShaderInterface {
protected:
	bool useGeometryShader;

	CGprofile vertexCGprofile;
	CGprofile geometryCGprofile;
	CGprofile pixelCGprofile;
	CGcontext cgContext;

	CGprogram geometryProgram;
	CGprogram vertexProgram;
	CGprogram fragmentProgram;

	TMesh* mesh;

public:
	ShaderInterface(bool useGeometryShader = false);

	void InitProfiles();
	void EnableProfiles();
	void DisableProfiles();
	void BindPrograms();
	void PerFrameDisable();

	bool loadGeometryProgram(char* filename, char* funcname);
	bool loadVertexProgram(char* filename, char* funcname);
	bool loadFragmentProgram(char* filename, char* funcname);

	void SetMesh(TMesh* mesh);

	virtual bool InitProgram() = 0;
	virtual void PerFrameInit() = 0;
};

