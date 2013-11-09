#pragma once

#include "V3.h"
#include "TIFFImage.h"
#include <vector>
#include <Cg/cgGL.h>
#include <Cg/cg.h>

class FrameBuffer;

class Texture {
public:
	GLuint texture_id;
	int width;
	int height;
	unsigned int* image;
	bool binded;

public:
	Texture();
	Texture(FrameBuffer* fb);
	Texture(const char* filename);
	~Texture();

	V3 GetColor(float s, float t);
	void SetColor(int u, int v, unsigned int c);

	unsigned int* GetImage();
	int GetWidth();
	int GetHeight();
	GLuint Bind();

private:
	V3 GetColor(unsigned int* image, int width, int height, float u, float v);
};

