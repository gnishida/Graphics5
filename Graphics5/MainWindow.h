#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include "FrameBuffer.h"
#include "V3.h"
#include "M33.h"
#include "TMesh.h"
#include "PPC.h"
#include "Texture.h"

// framebuffer + window class

class MainWindow : public Fl_Gl_Window {
public:
	FrameBuffer* frame;

	/** the last position of the mouse pointer */
	V3 lastPosition;

public:
	MainWindow(int u0, int v0, int _w, int _h); // constructor, top left coords and resolution
	~MainWindow();

	virtual void draw() = 0;

	// function called back when event occurs (mouse, keyboard, etc)
	int handle(int event);
	void KeyboardHandle();
	void mousePushHandle();
	void mouseMoveHandle();
};


