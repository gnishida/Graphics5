#pragma once

#include "gui.h"
#include "MainWindow.h"
#include "V3.h"
#include "M33.h"
#include "PPC.h"
#include "TMesh.h"
#include "Light.h"
#include <vector>
#include <iostream>

#define SCREEN_SPACE_RASTERIZATION	0
#define MODEL_SPACE_RASTERIZATION	1

#define NO_SHADING					0
#define GOURAUD_SHADING				1
#define PHONG_SHADING				2

using namespace std;

class Scene {
public:
	/** Main window with software buffer */
	MainWindow *win;

	/** Planar pinhole camera */
	vector<PPC*> ppcs;
	PPC* currentPPC;

	/** Graphical user interface */
	GUI *gui;

	/** Triangle mesh object */
	vector<TMesh*> meshes;

	static vector<Light*>* lights;

	bool mipmap_mode;

	float blueHue;
	float vertexDF;
	float geometryEF;

public:
	Scene();
	void Demo();
	void Save();
	void ProjectorDemo();
	void SaveProjector();
	void Render();
	void RenderProjectiveTextureMapping(FrameBuffer* fb, PPC* ppc);
	void RenderShadowMapping(FrameBuffer* fb, PPC* ppc);
	TIFFImage* CreateCubeMap(int size);
	V3 RayTrace(PPC* ppc, const V3 &p, const V3 &dir, float &dist);
};

extern Scene *scene;

