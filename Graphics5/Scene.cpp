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
#include <time.h>
#include <float.h>
#include <iostream>
#include <fstream>

using namespace std;

#define HIGH_RES		1

Scene *scene;
Light* Scene::light = new Light(V3(200.0f, 100.0f, 0.0f), Light::TYPE_POINT_LIGHT, 0.4f, 0.6f, 40.0f);

Scene::Scene() {
	mipmap_mode = true;

	// create user interface
	gui = new GUI();
	gui->show();

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
	
	// create a camera
	PPC* ppc = new PPC(60.0f, win->frame->w, win->frame->h);
	ppc->LookAt(V3(0.0f, 0.0f, 0.0f), V3(0.0f, -1.0f, -1.0f), V3(0.0f, 1.0f, -1.0f), 150.0f);
	ppcs.push_back(ppc);

	currentPPC = ppcs[0];

	//RenderHW();
	//Save();
}

/**
 * This function is called when "Demo" button is clicked.
 */
void Scene::Demo() {
	// Create a scene
	TMesh* mesh = new Box(V3(-40.0f, 0.0f, -10.0f), V3(-20.0f, 20.0f, 10.0f), V3(1.0f, 0.0f, 0.0f));
	meshes.push_back(mesh);

	mesh = new Box(V3(-10.0f, 0.0f, -40.0f), V3(10.0f, 20.0f, -20.0f), V3(0.0f, 1.0f, 0.0f));
	meshes.push_back(mesh);

	mesh = new Box(V3(20.0f, 0.0f, -10.0f), V3(40.0f, 20.0f, 10.0f), V3(0.0f, 0.0f, 1.0f));
	meshes.push_back(mesh);

	mesh = new Quad(100, 100, V3(0.0f, 1.0f, 1.0f));
	mesh->RotateAbout(V3(1.0f, 0.0f, 0.0f), -90.0f);
	meshes.push_back(mesh);

	for (int i = 0; i < 1000; i++) {
		//light->ambient = 0.4f + cosf((float)i * M_PI * 0.005) * 0.4f;
		//light->diffuse = 0.5f + cosf((float)i * M_PI * 0.005) * 0.5f;
		
		//light->position[0] += (i % 200 < 100) ? 1.0f : -1.0f;

		/*
		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		*/

		Render();
		Fl::wait();
	}

	// release the memory
	/*
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
	meshes.clear();
	*/
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
	// create a camera for the projective texture or shadow mapping
	PPC* ppc2 = new PPC(60.0f, win->frame->w, win->frame->h);
	ppc2->LookAt(V3(0.0f, 0.0f, 0.0f), V3(-1.0f, -1.0f, -1.0f), V3(-1.0f, 1.0f, -1.0f), 150.0f);

	// put an object in the scene
	TMesh* mesh = new TMesh();
	mesh->Load("geometry/teapot1K.bin");
	mesh->SetTexture("texture/stencil.tif");
	//mesh->SetProjectiveTexture(new ProjectiveTexture(ppc2, new Texture("texture/web.tif")));
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
	meshes.push_back(mesh2);



	// release the memory
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
	meshes.clear();
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

void Scene::CreateScene1() {

}