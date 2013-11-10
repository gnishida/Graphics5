#include "ShaderInterface.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

ShaderInterface::ShaderInterface(bool useGeometryShader) {
	this->useGeometryShader = useGeometryShader;
}

void ShaderInterface::InitProfiles() {
	glEnable(GL_DEPTH_TEST);

	if (useGeometryShader) {
		CGprofile latestGeometryProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);

		if (latestGeometryProfile == CG_PROFILE_UNKNOWN) {
			cerr << "ERROR: geometry profile is not available" << endl;
			exit(0);
		}

		cgGLSetOptimalOptions(latestGeometryProfile);
		CGerror Error = cgGetError();
		if (Error) {
			cerr << "CG ERROR: " << cgGetErrorString(Error) << endl;
		}

		cout << "Info: Latest GP Profile Supported: " << cgGetProfileString(latestGeometryProfile) << endl;

		geometryCGprofile = latestGeometryProfile;
	}

	CGprofile latestVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	CGprofile latestPixelProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

	cout << "Info: Latest VP Profile Supported: " << cgGetProfileString(latestVertexProfile) << endl;
	cout << "Info: Latest FP Profile Supported: " << cgGetProfileString(latestPixelProfile) << endl;

	vertexCGprofile = latestVertexProfile;
	pixelCGprofile = latestPixelProfile;
	cgContext = cgCreateContext();
}

void ShaderInterface::EnableProfiles() {
	if (useGeometryShader) {
		cgGLEnableProfile(geometryCGprofile);
	}
	cgGLEnableProfile(vertexCGprofile);
	cgGLEnableProfile(pixelCGprofile);
}

void ShaderInterface::DisableProfiles() {
	if (useGeometryShader) {
		cgGLDisableProfile(geometryCGprofile);
	}
	cgGLDisableProfile(vertexCGprofile);
	cgGLDisableProfile(pixelCGprofile);
}

void ShaderInterface::BindPrograms() {
	if (useGeometryShader) {
		cgGLBindProgram(geometryProgram);
	}
	cgGLBindProgram(vertexProgram);
	cgGLBindProgram(fragmentProgram);

	cgSetParameterSettingMode(cgContext, CG_IMMEDIATE_PARAMETER_SETTING);
}

void ShaderInterface::PerFrameDisable() {
}

bool ShaderInterface::loadGeometryProgram(char* filename, char* funcname) {
	geometryProgram = cgCreateProgramFromFile(cgContext, CG_SOURCE, filename, geometryCGprofile, funcname, NULL);

	if (geometryProgram == NULL) {
		CGerror Error = cgGetError();
		std::cerr << "Shader Geometry Program COMPILE ERROR: " << cgGetErrorString(Error) << std::endl;
		std::cerr << cgGetLastListing(cgContext) << std::endl << std::endl;
		return false;
	}

	cgGLLoadProgram(geometryProgram);

	return true;
}

bool ShaderInterface::loadVertexProgram(char* filename, char* funcname) {
	vertexProgram = cgCreateProgramFromFile(cgContext, CG_SOURCE, filename, vertexCGprofile, funcname, NULL);

	if (vertexProgram == NULL) {
		CGerror Error = cgGetError();
		std::cerr << "Shader Vertex Program COMPILE ERROR: " << cgGetErrorString(Error) << std::endl;
		std::cerr << cgGetLastListing(cgContext) << std::endl << std::endl;
		return false;
	}

	cgGLLoadProgram(vertexProgram);

	return true;
}

bool ShaderInterface::loadFragmentProgram(char* filename, char* funcname) {
	fragmentProgram = cgCreateProgramFromFile(cgContext, CG_SOURCE, filename, pixelCGprofile, funcname, NULL);

	if (fragmentProgram == NULL)  {
		CGerror Error = cgGetError();
		std::cerr << "Shader Fragment Program COMPILE ERROR: " << cgGetErrorString(Error) << std::endl;
		std::cerr << cgGetLastListing(cgContext) << std::endl << std::endl;
		return false;
	}

	cgGLLoadProgram(fragmentProgram);

	return true;
}
