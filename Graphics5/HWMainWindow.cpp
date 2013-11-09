#include "HWMainWindow.h"
#include "Scene.h"
#include "ShaderTextureInterface.h"
#include "ShaderPhongShadingInterface.h"
#include "ShaderProjectorInterface.h"
#include "ShaderProjectiveTextureInterface.h"
#include "ShaderShadowMappingInterface.h"

HWMainWindow::HWMainWindow(int u0, int v0, int _w, int _h) : MainWindow(u0, v0, _w, _h) {
	si = NULL;
}

void HWMainWindow::draw() {
}

void HWMainWindow::Render(std::vector<TMesh*>* meshes, PPC* ppc) {
	glEnable(GL_DEPTH_TEST);

	if (!si) {
		//si = new ShaderTextureInterface();
		//si = new ShaderPhongShadingInterface();
		si = new ShaderProjectiveTextureInterface();
		//si = new ShaderShadowMappingInterface();
		si->InitProfiles();
		si->InitProgram();
	}

	// frame setup
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// view setup
	float zNear = 1.0f;
	float zFar = 1000.0f;
	ppc->SetViewGL(zNear, zFar);

	if (si) {
		si->EnableProfiles();
		si->BindPrograms();
	}

	for (int i = 0; i < meshes->size(); i++) {
		if (meshes->at(i)->projector != NULL) {
			((ShaderProjectorInterface*)si)->SetPPC(meshes->at(i)->projector->ppc, zNear, zFar);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glBindTexture(GL_TEXTURE_2D, 123);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, meshes->at(i)->texture->GetWidth(), meshes->at(i)->texture->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, meshes->at(i)->texture->GetImage());
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, meshes->at(i)->texture->GetWidth(), meshes->at(i)->texture->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, meshes->at(i)->texture->GetImage());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		} else if (meshes->at(i)->texture != NULL) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glBindTexture(GL_TEXTURE_2D, 123);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, meshes->at(i)->texture->GetWidth(), meshes->at(i)->texture->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, meshes->at(i)->texture->GetImage());
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, meshes->at(i)->texture->GetWidth(), meshes->at(i)->texture->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, meshes->at(i)->texture->GetImage());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		if (si) {
			si->PerFrameInit();
		}

		meshes->at(i)->RenderHW();
	}

	if (si) {
		si->PerFrameDisable();
		si->DisableProfiles();
	}
}