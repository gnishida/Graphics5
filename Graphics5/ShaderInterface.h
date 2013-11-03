#pragma once

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

public:
	ShaderInterface(bool useGeometryShader = false);

	void InitProfiles();
	void EnableProfiles();
	void DisableProfiles();

	bool loadGeometryProgram(char* filename, char* funcname);
	bool loadVertexProgram(char* filename, char* funcname);
	bool loadFragmentProgram(char* filename, char* funcname);

	virtual bool InitProgram() = 0;
	virtual void BindPrograms() = 0;
	virtual void PerFrameInit() = 0;
	virtual void PerFrameDisable() = 0;
};

