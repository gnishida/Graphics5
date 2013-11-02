#pragma once

#include "MainWindow.h"

class SWMainWindow : public MainWindow{
public:
	SWMainWindow(int u0, int v0, int _w, int _h);

	void draw();
	void Render(std::vector<TMesh*>* meshes, PPC* ppc);
};

