#include "HWMainWindow.h"
#include "Scene.h"

HWMainWindow::HWMainWindow(int u0, int v0, int _w, int _h) : MainWindow(u0, v0, _w, _h) {
	soi = NULL;
	cgi = NULL;
}

void HWMainWindow::draw() {
}

void HWMainWindow::Render(std::vector<TMesh*>* meshes, PPC* ppc) {
	glEnable(GL_DEPTH_TEST);

	if (!cgi) {
		cgi = new CGInterface();
		cgi->PerSessionInit();
		soi = new ShaderOneInterface();
		soi->PerSessionInit(cgi);
	}

	// frame setup
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// view setup
	float zNear = 1.0f;
	float zFar = 1000.0f;
	ppc->SetViewGL(zNear, zFar);

	if (cgi) {
		cgi->EnableProfiles();
		soi->PerFrameInit();
		soi->BindPrograms();
	}

	for (int i = 0; i < meshes->size(); i++) {
		if (meshes->at(i)->texture != NULL) {
			glBindTexture(GL_TEXTURE_2D, 666);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, meshes->at(i)->texture->GetWidth(), meshes->at(i)->texture->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, meshes->at(i)->texture->GetImage());
			//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, meshes->at(i)->texture->GetWidth(), meshes->at(i)->texture->GetWidth(), GL_RGBA, GL_UNSIGNED_BYTE, meshes->at(i)->texture->GetImage());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		meshes->at(i)->RenderHW();
	}

	if (cgi) {
		soi->PerFrameDisable();
		cgi->DisableProfiles();
	}
}