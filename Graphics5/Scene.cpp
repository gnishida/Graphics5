#include "Scene.h"
#include "HWMainWindow.h"
#include "V3.h"
#include "Box.h"
#include "Triangle.h"
#include "Quad.h"
#include "Sphere.h"
#include "Light.h"
#include "FrameBuffer.h"
#include "ProjectiveTexture.h"
#include "ShadowMap.h"
#include "SoftShadowMap.h"
#include <time.h>
#include <float.h>
#include <iostream>
#include <fstream>

using namespace std;

#define HIGH_RES		1

Scene *scene;
Light* Scene::light = new Light(V3(-30.0f, 0.0f, -30.0f), Light::TYPE_DIRECTIONAL_LIGHT, 0.4f, 0.6f, 40.0f);

Scene::Scene() {
	mipmap_mode = true;

	// create user interface
	gui = new GUI();
	gui->show();

	// create SW framebuffer
	int u0 = 20;
	int v0 = 50;
	int sci = 2;
	int w = sci*240;//640;
	int h = sci*180;//360;

	// create HW framebuffer
	win = new HWMainWindow(u0, v0, w, h);
	win->label("HW Framebuffer");
	win->show();
  
	// position UI window
	gui->uiw->position(win->frame->w+u0 + 2*20, v0);
	
	// create a camera for the projective texture or shadow mapping
	PPC* ppc2 = new PPC(60.0f, win->frame->w, win->frame->h);
	ppc2->LookAt(V3(0.0f, 0.0f, 0.0f), V3(-1.0f, -1.0f, -1.0f), V3(-1.0f, 1.0f, -1.0f), 150.0f);

	// craete a soft shadow map
	SoftShadowMap* ssm = new SoftShadowMap(V3(100.0f, 100.0f, 100.0f), V3(-1.0f, -1.0f, -1.0f), 10.0f, 2);
	//SoftShadowMap* ssm = new SoftShadowMap(V3(100.0f, 100.0f, 100.0f), V3(-1.0f, -1.0f, -1.0f), 10.0f, 16);
	
	// put an object in the scene
	TMesh* mesh = new TMesh();
	mesh->Load("geometry/teapot1K.bin");
	mesh->SetTexture("texture/stencil.tif");
	//mesh->SetProjectiveTexture(new ProjectiveTexture(ppc2, new Texture("texture/web.tif")));
	//mesh->SetShadowMap(new ShadowMap(ppc2, w, h));
	//mesh->SetSoftShadowMap(ssm);
	mesh->Translate(mesh->GetCentroid() * -1.0f);
	//meshes.push_back(mesh);

	mesh = new Box(V3(-20.0f, -20.0f, -20.0f), V3(20.0f, 20.0f, 20.0f), V3(0.0f, 1.0f, 0.0f));
	mesh->SetTexture("texture/stencil.tif");
	meshes.push_back(mesh);

	TMesh* mesh2 = new Quad(100, 100, V3(0.0f, 0.0f, 1.0f));
	mesh2->RotateAbout(V3(1.0f, 0.0f, 0.0f), -90.0f);
	mesh2->Translate(V3(0.0f, -26.0f, 0.0f));
	mesh2->SetTexture("texture/stencil.tif");
	//mesh->SetProjectiveTexture(new ProjectiveTexture(ppc2, new Texture("texture/web.tif")));
	//mesh->SetShadowMap(new ShadowMap(ppc2, w, h));
	//mesh->SetSoftShadowMap(ssm);
	meshes.push_back(mesh2);

	// create a camera
	PPC* ppc = new PPC(60.0f, win->frame->w, win->frame->h);
	ppc->LookAt(V3(0.0f, 0.0f, 0.0f), V3(0.0f, 0.0f, -1.0f), V3(0.0f, 1.0f, 0.0f), 150.0f);
	ppcs.push_back(ppc);

	currentPPC = ppcs[0];

	//RenderHW();
	//Save();
}

/**
 * This function is called when "Demo" button is clicked.
 */
void Scene::Demo() {
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < meshes.size(); j++) {
			meshes[j]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.1f);
		}

		Render();
		Fl::wait();
	}
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
	win->Render(&meshes, currentPPC);
	win->redraw();
}

void Scene::RenderProjectiveTextureMapping(FrameBuffer* fb, PPC* ppc) {
	win->frame->SetZB(0.0f);
	win->frame->Set(WHITE);

	// projective texture mapping
	for (int i = 0; i < meshes.size(); i++) {
		//meshes[i]->RenderProjectiveTextureMapping(fb, ppc, win->frame, currentPPC);
	}

	win->redraw();
}

void Scene::RenderShadowMapping(FrameBuffer* fb, PPC* ppc) {
	win->frame->SetZB(0.0f);
	win->frame->Set(WHITE);

	for (int i = 0; i < meshes.size(); i++) {
		//meshes[i]->RenderShadowMapping(fb, ppc, win->frame, currentPPC);
	}

	win->redraw();
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