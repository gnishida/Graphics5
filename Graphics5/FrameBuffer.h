#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include "V3.h"
#include "M33.h"
#include "TMesh.h"
#include "PPC.h"
#include "Texture.h"

// framebuffer + window class

class FrameBuffer {
public:
	/** software color buffer (The first pixel is the bottom left corner.) */
	unsigned int *pix;

	/** software Z buffer */
	float *zb;

	/** image wdith resolution */
	int w;
		
	/** image height resolution */
	int h;

public:
	FrameBuffer(int _w, int _h); // constructor, top left coords and resolution
	~FrameBuffer();

	void Set(unsigned int bgr);
	void Set(int u, int v, unsigned int clr);
	void Set(int u, int v, unsigned int clr, float z);
	void SetGuarded(int u, int v, unsigned int clr, float z);
	void SetZB(float z0);
	void Draw2DSegment(const V3 &p0, const V3 &c0, const V3 &p1, const V3 &c1);
	void Draw3DSegment(PPC* ppc, const V3 &p0, const V3 &c0, const V3 &p1, const V3 &c1);
	void DrawRectangle(const V3 &p0, const V3 &p1, const V3 &c);
	bool Load(char* filename);
	bool Save(char* filename);

	void Draw2DBigPoint(int u, int v, int psize, const V3 &color, float z);
	void Draw3DBigPoint(PPC* ppc, const V3 &p, int psize, const V3 &color);

	bool isHidden(int u, int v, float z);
	bool isCompletelyHidden(float u, float v, float z);
	bool isPartiallyHidden(float u, float v, float z);

	V3 Get(int u, int v);
	V3 Get(float u, float v);
	V3 GetNearestColor(int u, int v);
	float GetZ(int u, int v);
};


