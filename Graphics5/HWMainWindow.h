#pragma once

#include "MainWindow.h"
#include "ShaderInterface.h"

class HWMainWindow : public MainWindow {
private:
	/** for hw rendering */
	//CGInterface *cgi;
	ShaderInterface *si;

public:
	HWMainWindow(int u0, int v0, int _w, int _h);

	void draw();
	void Render(std::vector<TMesh*>* meshes, PPC* ppc);
};

