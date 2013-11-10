#include "HWMainWindow.h"
#include "Scene.h"
#include "ShaderTextureInterface.h"
#include "ShaderPhongShadingInterface.h"
#include "ShaderProjectorInterface.h"
#include "ShaderProjectiveTextureInterface.h"
#include "ShaderShadowMappingInterface.h"
#include "ShaderSoftShadowMappingInterface.h"
#include "ShaderStencilTextureInterface.h"

HWMainWindow::HWMainWindow(int u0, int v0, int _w, int _h) : MainWindow(u0, v0, _w, _h) {
	si = NULL;
}

void HWMainWindow::draw() {
}

void HWMainWindow::Render(std::vector<TMesh*>* meshes, PPC* ppc) {
	glEnable(GL_DEPTH_TEST);

	// frame setup
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!si) {
		//si = new ShaderTextureInterface();
		//si = new ShaderPhongShadingInterface();
		//si = new ShaderProjectiveTextureInterface();
		//si = new ShaderShadowMappingInterface();
		si = new ShaderSoftShadowMappingInterface();
		//si = new ShaderStencilTextureInterface();
		si->InitProfiles();
		si->InitProgram();
	}

	if (si) {
		si->EnableProfiles();
		si->BindPrograms();
	}

	// view setup
	float zNear = 1.0f;
	float zFar = 1000.0f;
	ppc->SetViewGL(zNear, zFar);

	for (int i = 0; i < meshes->size(); i++) {
		if (meshes->at(i)->softShadowMap != NULL) {
		} else if (meshes->at(i)->shadowMap != NULL) {
			((ShaderShadowMappingInterface*)si)->SetShadowMap(meshes->at(i)->shadowMap, meshes);
		} else if (meshes->at(i)->projTexture != NULL) {
			((ShaderProjectiveTextureInterface*)si)->SetProjTexture(meshes->at(i)->projTexture);
		} else if (meshes->at(i)->texture != NULL) {
			((ShaderTextureInterface*)si)->SetTexture(meshes->at(i)->texture);
		}

		si->PerFrameInit();

		meshes->at(i)->RenderHW();
	}

	if (si) {
		si->PerFrameDisable();
		si->DisableProfiles();
	}
}