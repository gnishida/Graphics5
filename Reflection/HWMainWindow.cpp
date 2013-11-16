#include "HWMainWindow.h"
#include "Scene.h"
#include "ShaderCubeMappingInterface.h"

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
		si = new ShaderCubeMappingInterface();
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

	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, frame->w, frame->h, GL_RGBA, GL_UNSIGNED_BYTE, frame->pix);
}