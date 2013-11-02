#include "Scene.h"
#include "SWMainWindow.h"
#include "HWMainWindow.h"
#include "V3.h"
#include "Box.h"
#include "Triangle.h"
#include "Quad.h"
#include "Sphere.h"
#include "Light.h"
#include "FrameBuffer.h"
#include "Projector.h"
#include <time.h>
#include <float.h>
#include <iostream>
#include <fstream>

using namespace std;

#define HIGH_RES		1

Scene *scene;// = new Scene();
std::vector<Light*>* Scene::lights = new std::vector<Light*>();

Scene::Scene() {
	soi = NULL;
	cgi = NULL;

	mipmap_mode = true;

	//lights.push_back(new Light(V3(30.0f, 0.0f, 30.0f), Light::TYPE_POINT_LIGHT, 0.4f, 0.6f, 40.0f));
	lights->push_back(new Light(V3(-30.0f, 0.0f, -30.0f), Light::TYPE_DIRECTIONAL_LIGHT, 0.4f, 0.6f, 40.0f));
	//lights.push_back(new Light(V3(-30.0f, 0.0f, 0.0f), Light::TYPE_DIRECTIONAL_LIGHT, 0.4f, 0.6f, 40.0f));

	// create user interface
	gui = new GUI();
	gui->show();

	// create SW framebuffer
	int u0 = 20;
	int v0 = 50;
	int sci = 2;
	int w = sci*240;//640;
	int h = sci*180;//360;
	/*
	win = new SWMainWindow(u0, v0, w, h);
	win->label("SW Framebuffer");
	win->show();
	*/

	// create HW framebuffer
	win = new HWMainWindow(u0, v0, w, h);
	win->label("HW Framebuffer");
	win->show();
  
	// position UI window
	gui->uiw->position(win->frame->w+u0 + 2*20, v0);
	
	// put an object in the scene
	TMesh* mesh = new Box(V3(-10.0f, -10.0f, -10.0f), V3(10.0f, 10.0f, 10.0f), V3(1.0f, 0.0f, 0.0f));
	meshes.push_back(mesh);

	// create a camera
	PPC* ppc = new PPC(60.0f, win->frame->w, win->frame->h);
	ppc->LookAt(V3(0.0f, 0.0f, 0.0f), V3(0.0f, 0.0f, -1.0f), V3(0.0f, 1.0f, 0.0f), 100.0f);
	ppcs.push_back(ppc);

	currentPPC = ppcs[0];

	//RenderHW();
	//Save();
}

/**
 * This function is called when "Demo" button is clicked.
 */
void Scene::Demo() {
	RenderHW();
}

/**
 * This function is called when "Save" button is clicked.
 * The scene is animated with total 600 frames, which are corresponding to 20 seconds of animation,
 * and all the frames are stored in "captured" folder.
 */
void Scene::Save() {
	
}

/**
 * Projector Demo
 */
void Scene::ProjectorDemo() {
}

/**
 * Save the project demo to tiff files.
 */
void Scene::SaveProjector() {
}

/**
 * Render all the models.
 */
void Scene::Render() {
	win->frame->SetZB(0.0f);
	win->frame->Set(WHITE);

	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->Render(win->frame, currentPPC);
	}

	win->redraw();
}

void Scene::RenderHW() {
  glEnable(GL_DEPTH_TEST);

  if (!cgi) {
    cgi = new CGInterface();
    cgi->PerSessionInit();
    soi = new ShaderOneInterface();
    soi->PerSessionInit(cgi);
  }

  // frame setup
  glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // view setup
  float zNear = 1.0f;
  float zFar = 1000.0f;
  currentPPC->SetViewGL(zNear, zFar);

  if (cgi) {
    cgi->EnableProfiles();
    soi->PerFrameInit();
    soi->BindPrograms();
  }

	for (int tmi = 0; tmi < meshes.size(); tmi++) {
		meshes[tmi]->RenderHW();
	}

  if (cgi) {
    soi->PerFrameDisable();
    cgi->DisableProfiles();
  }

}

void Scene::RenderProjectiveTextureMapping(FrameBuffer* fb, PPC* ppc) {
	win->frame->SetZB(0.0f);
	win->frame->Set(WHITE);

	// projective texture mapping
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->RenderProjectiveTextureMapping(fb, ppc, win->frame, currentPPC);
	}

	win->redraw();
}

void Scene::RenderShadowMapping(FrameBuffer* fb, PPC* ppc) {
	win->frame->SetZB(0.0f);
	win->frame->Set(WHITE);

	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->RenderShadowMapping(fb, ppc, win->frame, currentPPC);
	}

	win->redraw();
}

TIFFImage* Scene::CreateCubeMap(int size) {
	PPC ppc(90.0f, size, size);

	// render the back scene
	FrameBuffer fb1(size, size);
	fb1.SetZB(0.0f);
	fb1.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb1, &ppc);
	}

	// render the left scene
	FrameBuffer fb2(size, size);
	ppc.Pan(90.0f);
	fb2.SetZB(0.0f);
	fb2.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb2, &ppc);
	}

	// render the front scene
	FrameBuffer fb3(size, size);
	ppc.Pan(90.0f);
	fb3.SetZB(0.0f);
	fb3.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb3, &ppc);
	}

	// render the right scene
	FrameBuffer fb4(size, size);
	ppc.Pan(90.0f);
	fb4.SetZB(0.0f);
	fb4.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb4, &ppc);
	}

	// render the top scene
	FrameBuffer fb5(size, size);
	ppc.Pan(90.0f);
	ppc.Tilt(90.0f);
	fb5.SetZB(0.0f);
	fb5.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb5, &ppc);
	}

	// render the bottom scene
	FrameBuffer fb6(size, size);
	ppc.Tilt(-180.0f);
	fb6.SetZB(0.0f);
	fb6.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb6, &ppc);
	}

	int w = size * 3;
	int h = size * 4;
	TIFFImage* result = new TIFFImage(w, h);
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			result->pix[x + size + y * w] = fb1.pix[x + (size - y - 1) * size];
			result->pix[x + (y + size * 2) * w] = fb2.pix[(size - x - 1) + y * size];
			result->pix[x + size + (y + size * 2) * w] = fb3.pix[(size - x - 1) + y * size];
			result->pix[x + size * 2 + (y + size * 2) * w] = fb4.pix[(size - x - 1) + y * size];
			result->pix[x + size + (y + size * 3) * w] = fb5.pix[(size - x - 1) + y * size];
			result->pix[x + size + (y + size) * w] = fb6.pix[x + (size - y - 1) * size];
		}
	}

	return result;
}

V3 Scene::RayTrace(PPC* ppc, const V3 &p, const V3 &dir, float &dist) {
	float dist_min = std::numeric_limits<float>::max();
	V3 c_min;

	for (int i = 0; i < meshes.size(); i++) {
		float d;
		V3 c;
		if (!meshes[i]->RayTrace(ppc, p, dir, c, d)) continue;
		if (d < dist_min) {
			dist_min = d;
			c_min = c;
		}
	}

	dist = dist_min;

	return c_min;
}