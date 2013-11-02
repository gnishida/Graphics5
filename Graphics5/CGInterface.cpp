//#define GEOM_SHADER

#include "CGInterface.h"
#include "v3.h"
#include "scene.h"

#include <iostream>

using namespace std;

CGInterface::CGInterface() {};

void CGInterface::PerSessionInit() {
	glEnable(GL_DEPTH_TEST);

	CGprofile latestVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	CGprofile latestGeometryProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);
	CGprofile latestPixelProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

	if (latestGeometryProfile == CG_PROFILE_UNKNOWN) {
		cerr << "ERROR: geometry profile is not available" << endl;
#ifdef GEOM_SHADER
		exit(0);
#endif
	}

	cgGLSetOptimalOptions(latestGeometryProfile);
	CGerror Error = cgGetError();
	if (Error) {
		cerr << "CG ERROR: " << cgGetErrorString(Error) << endl;
	}

	cout << "Info: Latest GP Profile Supported: " << cgGetProfileString(latestGeometryProfile) << endl;

	geometryCGprofile = latestGeometryProfile;

	cout << "Info: Latest VP Profile Supported: " << cgGetProfileString(latestVertexProfile) << endl;
	cout << "Info: Latest FP Profile Supported: " << cgGetProfileString(latestPixelProfile) << endl;

	vertexCGprofile = latestVertexProfile;
	pixelCGprofile = latestPixelProfile;
	cgContext = cgCreateContext();
}

void CGInterface::EnableProfiles() {
	cgGLEnableProfile(vertexCGprofile);
#ifdef GEOM_SHADER
	cgGLEnableProfile(geometryCGprofile);
#endif
	cgGLEnableProfile(pixelCGprofile);
}

void CGInterface::DisableProfiles() {
	cgGLDisableProfile(vertexCGprofile);
#ifdef GEOM_SHADER
	cgGLDisableProfile(geometryCGprofile);
#endif
	cgGLDisableProfile(pixelCGprofile);
}


bool ShaderOneInterface::PerSessionInit(CGInterface *cgi) {
#ifdef GEOM_SHADER
	geometryProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, "CG/shaderOne.cg", cgi->geometryCGprofile, "GeometryMain", NULL);
	if (geometryProgram == NULL)  {
		CGerror Error = cgGetError();
		cerr << "Shader One Geometry Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
		cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
		return false;
	}
#endif

	vertexProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, "CG/shaderOne.cg", cgi->vertexCGprofile, "VertexMain", NULL);
	if (vertexProgram == NULL) {
		CGerror Error = cgGetError();
		cerr << "Shader One Geometry Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
		cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
		return false;
	}

	fragmentProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, "CG/shaderOne.cg", cgi->pixelCGprofile, "FragmentMain", NULL);
	if (fragmentProgram == NULL)  {
		CGerror Error = cgGetError();
		cerr << "Shader One Fragment Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
		cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
		return false;
	}

	// load programs
#ifdef GEOM_SHADER
	cgGLLoadProgram(geometryProgram);
#endif
	cgGLLoadProgram(vertexProgram);
	cgGLLoadProgram(fragmentProgram);

	// build some parameters by name such that we can set them later...
	//geometryEF = cgGetNamedParameter(geometryProgram, "ef" );
	//geometryModelViewProj = cgGetNamedParameter(geometryProgram, "modelViewProj" );
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj" );
	vertexDF = cgGetNamedParameter(vertexProgram, "df" );
	fragmentBlueHue = cgGetNamedParameter(fragmentProgram, "blueHue" );
	fragmentEye = cgGetNamedParameter(fragmentProgram, "eye" );
	fragmentTeapotCenter = cgGetNamedParameter(fragmentProgram, "teapotCenter" );

	return true;
}

void ShaderOneInterface::PerFrameInit() {
	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetParameter1f(vertexDF, scene->vertexDF);

	//cgGLSetParameter1f(geometryEF, scene->geometryEF);
	//cgGLSetStateMatrixParameter(geometryModelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	cgGLSetParameter1f(fragmentBlueHue, scene->blueHue);
	cgGLSetParameter3fv(fragmentEye, (float*) &(scene->currentPPC->C));
	V3 teapotCenter = scene->meshes[0]->GetCentroid();
	cgGLSetParameter3fv(fragmentTeapotCenter, (float*)&teapotCenter);

#if 0
  scene->diPPC->SetIntrinsicsHW();
  scene->diPPC->SetExtrinsicsHW();
  cgGLSetStateMatrixParameter(fragmentDIMVP,
		CG_GL_MODELVIEW_PROJECTION_MATRIX, 
		CG_GL_MATRIX_IDENTITY);
  scene->ppc->SetIntrinsicsHW();
  scene->ppc->SetExtrinsicsHW();
#endif
}

void ShaderOneInterface::PerFrameDisable() {
}


void ShaderOneInterface::BindPrograms() {
#ifdef GEOM_SHADER
	cgGLBindProgram(geometryProgram);
#endif
	cgGLBindProgram( vertexProgram);
	cgGLBindProgram( fragmentProgram);
}

