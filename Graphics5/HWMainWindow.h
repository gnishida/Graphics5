#pragma once

#include "MainWindow.h"
#include "CGInterface.h"

class HWMainWindow : public MainWindow {
private:
	/** for hw rendering */
	CGInterface *cgi;
	ShaderOneInterface *soi;

public:
	HWMainWindow(int u0, int v0, int _w, int _h);

	void draw();
	void Render(std::vector<TMesh*>* meshes, PPC* ppc);
};

