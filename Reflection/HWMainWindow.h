#pragma once

#include "MainWindow.h"
#include "ShaderInterface.h"

class HWMainWindow : public MainWindow {
private:
	/** for hw rendering */
	ShaderInterface *si;
	ShaderInterface *si2;

public:
	HWMainWindow(int u0, int v0, int _w, int _h);

	void draw();
	void Render(std::vector<TMesh*>* meshes, PPC* ppc);
};

