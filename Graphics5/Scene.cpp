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
#include <time.h>

using namespace std;

Scene *scene;
Light* Scene::light = new Light(V3(210.0f, 210.0f, 0.0f), Light::TYPE_POINT_LIGHT, 0.4f, 0.6f, 40.0f);
float Scene::light_step = 1.0f;

Scene::Scene() {
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

	// Create a scene
	TMesh* mesh = new Box(V3(-32.0f, 0.0f, -10.0f), V3(-12.0f, 20.0f, 10.0f), V3(1.0f, 0.0f, 0.0f));
	meshes.push_back(mesh);

	mesh = new Box(V3(-10.0f, 0.0f, -32.0f), V3(10.0f, 20.0f, -12.0f), V3(0.0f, 1.0f, 0.0f));
	meshes.push_back(mesh);

	mesh = new Box(V3(12.0f, 0.0f, -10.0f), V3(32.0f, 20.0f, 10.0f), V3(0.0f, 0.0f, 1.0f));
	meshes.push_back(mesh);

	mesh = new Quad(200, 200, V3(0.0f, 1.0f, 1.0f));
	mesh->RotateAbout(V3(1.0f, 0.0f, 0.0f), -90.0f);
	meshes.push_back(mesh);

	//RenderHW();
	//Save();
}

/**
 * This function is called when "Demo" button is clicked.
 */
void Scene::Demo() {
	time_t start , end;

	// (1) In the first 10 sec, only the boxes move around.
	for (int i = 0; i < 300; i++) {
		start = clock();

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));

		Render();
		Fl::wait();

		end = clock();
		fprintf(stdout, "FPS: %.1f\n", (double)CLOCKS_PER_SEC / (double)(end - start));
	}

	// (2) In the next 10 sec, the light grow and shrink.
	for (int i = 300; i < 600; i++) {
		start = clock();

		light_step += (i < 450) ? 0.1f : -0.1f;

		Render();
		Fl::wait();

		end = clock();
		fprintf(stdout, "FPS: %.1f\n", (double)CLOCKS_PER_SEC / (double)(end - start));
	}

	// (3) In the next 10 sec, the light and the boxes move around.
	for (int i = 600; i < 900; i++) {
		start = clock();

		light->position[0] -= 1.4f;

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));

		Render();
		Fl::wait();

		end = clock();
		fprintf(stdout, "FPS: %.1f\n", (double)CLOCKS_PER_SEC / (double)(end - start));
	}
}

/**
 * This function is called when "Save" button is clicked.
 * The scene is animated with total 600 frames, which are corresponding to 20 seconds of animation,
 * and all the frames are stored in "captured" folder.
 */
void Scene::Save() {
	char filename[256];

	// (1) In the first 10 sec, only the boxes move around.
	for (int i = 0; i < 300; i++) {
		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));

		Render();
		sprintf(filename, "captured\\scene%04d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}

	// (2) In the next 10 sec, the light grow and shrink.
	for (int i = 300; i < 600; i++) {
		light_step += (i < 450) ? 0.1f : -0.1f;

		Render();
		sprintf(filename, "captured\\scene%04d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}

	// (3) In the next 10 sec, the light and the boxes move around.
	for (int i = 600; i < 900; i++) {
		light->position[0] -= 1.4f;

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));

		Render();
		sprintf(filename, "captured\\scene%04d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}
}

/**
 * Projector Demo
 */
void Scene::StencilDemo() {
	time_t start , end;

	this->light->position[0] = -210.0f;

	// For the second half of the demo, apply the texture to the boxes
	for (int i = 0; i < 3; i++) {
		meshes[i]->SetTexture("texture/stencil.tif");
	}

	// (1) In the first 10 sec, only the boxes move around.
	for (int i = 900; i < 1200; i++) {
		start = clock();

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));

		Render();
		Fl::wait();

		end = clock();
		fprintf(stdout, "FPS: %.1f\n", (double)CLOCKS_PER_SEC / (double)(end - start));
	}

	// (2) In the next 10 sec, the light grow and shrink.
	for (int i = 1200; i < 1500; i++) {
		start = clock();

		light_step += (i < 1350) ? 0.1f : -0.1f;

		Render();
		Fl::wait();

		end = clock();
		fprintf(stdout, "FPS: %.1f\n", (double)CLOCKS_PER_SEC / (double)(end - start));
	}

	// (3) In the next 10 sec, the light and the boxes move around.
	for (int i = 1500; i < 1800; i++) {
		start = clock();

		light->position[0] += 1.4f;

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));

		Render();
		Fl::wait();

		end = clock();
		fprintf(stdout, "FPS: %.1f\n", (double)CLOCKS_PER_SEC / (double)(end - start));
	}
}

/**
 * Save the project demo to tiff files.
 */
void Scene::SaveStencil() {
	char filename[256];

	this->light->position[0] = -210.0f;

	// For the second half of the demo, apply the texture to the boxes
	for (int i = 0; i < 3; i++) {
		meshes[i]->SetTexture("texture/stencil.tif");
	}

	// (1) In the first 10 sec, only the boxes move around.
	for (int i = 900; i < 1200; i++) {
		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));

		Render();
		sprintf(filename, "captured\\scene%04d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}

	// (2) In the next 10 sec, the light grow and shrink.
	for (int i = 1200; i < 1500; i++) {
		light_step += (i < 1350) ? 0.1f : -0.1f;

		Render();
		sprintf(filename, "captured\\scene%04d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}

	// (3) In the next 10 sec, the light and the boxes move around.
	for (int i = 1500; i < 1800; i++) {
		light->position[0] += 1.4f;

		meshes[0]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[1]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));
		meshes[2]->RotateAbout(V3(0.0f, 1.0f, 0.0f), 1.2f, V3(0.0f, 0.0f, 0.0f));

		Render();
		sprintf(filename, "captured\\scene%04d.tif", i);
		win->frame->Save(filename);
		Fl::wait();
	}
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