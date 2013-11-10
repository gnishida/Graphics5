#include "HWMainWindow.h"
#include "Scene.h"
#include "ShaderStencilTextureWithSoftShadowMappingInterface.h"

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
		si = new ShaderStencilTextureWithSoftShadowMappingInterface();
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
		si->SetMesh(meshes->at(i));
		si->PerFrameInit();

		meshes->at(i)->RenderHW();
	}

	if (si) {
		si->PerFrameDisable();
		si->DisableProfiles();
	}
}