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
int Scene::light_step = 1;

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
	gui->uiw->position(w+u0 + 2*20, v0);
	
	// create a camera
	PPC* ppc = new PPC(60.0f, w, h);
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
	TMesh* mesh = new Box(V3(-40.0f, 0.0f, -10.0f), V3(-20.0f, 30.0f, 10.0f), V3(1.0f, 0.0f, 0.0f));
	meshes.push_back(mesh);

	mesh = new Box(V3(-10.0f, 0.0f, -40.0f), V3(10.0f, 20.0f, -20.0f), V3(0.0f, 1.0f, 0.0f));
	meshes.push_back(mesh);

	mesh = new Box(V3(20.0f, 0.0f, -10.0f), V3(40.0f, 10.0f, 10.0f), V3(0.0f, 0.0f, 1.0f));
	meshes.push_back(mesh);

	mesh = new Quad(100, 100, V3(0.0f, 1.0f, 1.0f));
	mesh->RotateAbout(V3(1.0f, 0.0f, 0.0f), -90.0f);
	meshes.push_back(mesh);

	// (1) In the first 10 sec, only the boxes move around.
	for (int i = 0; i < 300; i++) {
		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));

		Render();
		Fl::wait();
	}

	// (2) In the next 10 sec, the light grow and shrink.
	for (int i = 0; i < 300; i++) {
		(i < 150) ? light_step++ : light_step--;

		Render();
		Fl::wait();
	}

	// (3) In the next 10 sec, the light and the boxes move around.
	for (int i = 0; i < 300; i++) {
		light->position[0] += (i % 200 < 100) ? 1.0f : -1.0f;

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));

		Render();
		Fl::wait();
	}

	// For the second half of the demo, apply the texture to the boxes
	for (int i = 0; i < 3; i++) {
		meshes[i]->SetTexture("texture/stencil.tif");
	}

	// (1) In the first 10 sec, only the boxes move around.
	for (int i = 0; i < 300; i++) {
		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));

		Render();
		Fl::wait();
	}

	// (2) In the next 10 sec, the light grow and shrink.
	for (int i = 0; i < 300; i++) {
		(i < 150) ? light_step++ : light_step--;

		Render();
		Fl::wait();
	}

	// (3) In the next 10 sec, the light and the boxes move around.
	for (int i = 0; i < 300; i++) {
		light->position[0] += (i % 200 < 100) ? 1.0f : -1.0f;

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));

		Render();
		Fl::wait();
	}

	// release the memory
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
	meshes.clear();
}

/**
 * This function is called when "Save" button is clicked.
 * The scene is animated with total 600 frames, which are corresponding to 20 seconds of animation,
 * and all the frames are stored in "captured" folder.
 */
void Scene::Save() {
	char filename[256];

	// Create a scene
	TMesh* mesh = new Box(V3(-40.0f, 0.0f, -10.0f), V3(-20.0f, 30.0f, 10.0f), V3(1.0f, 0.0f, 0.0f));
	meshes.push_back(mesh);

	mesh = new Box(V3(-10.0f, 0.0f, -40.0f), V3(10.0f, 20.0f, -20.0f), V3(0.0f, 1.0f, 0.0f));
	meshes.push_back(mesh);

	mesh = new Box(V3(20.0f, 0.0f, -10.0f), V3(40.0f, 10.0f, 10.0f), V3(0.0f, 0.0f, 1.0f));
	meshes.push_back(mesh);

	mesh = new Quad(100, 100, V3(0.0f, 1.0f, 1.0f));
	mesh->RotateAbout(V3(1.0f, 0.0f, 0.0f), -90.0f);
	meshes.push_back(mesh);

	// (1) In the first 10 sec, only the boxes move around.
	for (int i = 0; i < 300; i++) {
		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));

		Render();
		sprintf(filename, "captured\\scene%03d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}

	// (2) In the next 10 sec, the light grow and shrink.
	for (int i = 300; i < 600; i++) {
		(i < 450) ? light_step++ : light_step--;

		Render();
		sprintf(filename, "captured\\scene%03d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}

	// (3) In the next 10 sec, the light and the boxes move around.
	for (int i = 600; i < 900; i++) {
		light->position[0] += (i % 200 < 100) ? 1.0f : -1.0f;

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));

		Render();
		sprintf(filename, "captured\\scene%03d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}

	// For the second half of the demo, apply the texture to the boxes
	for (int i = 0; i < 3; i++) {
		meshes[i]->SetTexture("texture/stencil.tif");
	}

	// (1) In the first 10 sec, only the boxes move around.
	for (int i = 900; i < 1200; i++) {
		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));

		Render();
		sprintf(filename, "captured\\scene%03d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}

	// (2) In the next 10 sec, the light grow and shrink.
	for (int i = 1200; i < 1500; i++) {
		(i < 1350) ? light_step++ : light_step--;

		Render();
		sprintf(filename, "captured\\scene%03d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}

	// (3) In the next 10 sec, the light and the boxes move around.
	for (int i = 1500; i < 1800; i++) {
		light->position[0] += (i % 200 < 100) ? 1.0f : -1.0f;

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));

		Render();
		sprintf(filename, "captured\\scene%03d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}

	// release the memory
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
	meshes.clear();
}

/**
 * Projector Demo
 */
void Scene::ProjectorDemo() {
	// Create a scene
	TMesh* mesh = new Box(V3(-40.0f, 0.0f, -10.0f), V3(-20.0f, 20.0f, 10.0f), V3(1.0f, 0.0f, 0.0f));
	mesh->SetTexture("texture/stencil.tif");
	meshes.push_back(mesh);
	
	mesh = new Box(V3(-10.0f, 0.0f, -40.0f), V3(10.0f, 20.0f, -20.0f), V3(0.0f, 1.0f, 0.0f));
	mesh->SetTexture("texture/stencil.tif");
	meshes.push_back(mesh);

	mesh = new Box(V3(20.0f, 0.0f, -10.0f), V3(40.0f, 20.0f, 10.0f), V3(0.0f, 0.0f, 1.0f));
	mesh->SetTexture("texture/stencil.tif");
	meshes.push_back(mesh);

	mesh = new Quad(100, 100, V3(0.0f, 1.0f, 1.0f));
	mesh->RotateAbout(V3(1.0f, 0.0f, 0.0f), -90.0f);
	meshes.push_back(mesh);

	for (int i = 0; i < 1000; i++) {
		//light->ambient = 0.4f + cosf((float)i * M_PI * 0.005) * 0.4f;
		//light->diffuse = 0.5f + cosf((float)i * M_PI * 0.005) * 0.5f;
		
		//light->position[0] += (i % 200 < 100) ? 1.0f : -1.0f;

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.4f, V3(0.0f, 0.0f, 0.0f));

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