#include "SWMainWindow.h"

SWMainWindow::SWMainWindow(int u0, int v0, int _w, int _h) : MainWindow(u0, v0, _w, _h) {
}

void SWMainWindow::draw() {
	glDrawPixels(frame->w, frame->h, GL_RGBA, GL_UNSIGNED_BYTE, frame->pix);
}

void SWMainWindow::Render(std::vector<TMesh*>* meshes, PPC* ppc) {
	frame->SetZB(0.0f);
	frame->Set(WHITE);

	for (int i = 0; i < meshes->size(); i++) {
		meshes->at(i)->Render(frame, ppc);
	}

	redraw();
}