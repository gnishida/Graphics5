#include "Texture.h"
#include "Scene.h"
#include "FrameBuffer.h"
#include <libtiff/tiffio.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#define log2f(x)		(logf(x) / logf(2.0f))

Texture::Texture() {
	binded = false;
}

Texture::Texture(const char* filename) {
	TIFF* tiff = TIFFOpen(filename, "r");
	if (tiff == NULL) throw "File is not accessible.";

	int w, h;

	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &h);

	image = (unsigned int*)_TIFFmalloc(sizeof(unsigned int) * w * h);
	width = w;
	height = h;
	if (!TIFFReadRGBAImage(tiff, w, h, image, 0)) {
		delete [] image;
		image = NULL;
		TIFFClose(tiff);
		return;
	}

	TIFFClose(tiff);

	binded = false;
}

Texture::~Texture() {
	delete [] image;
}

void Texture::SetColor(int u, int v, unsigned int c) {
	if (u < 0 || u >= width) return;
	if (v < 0 || v >= height) return;

	image[u + v * width] = c;
}

unsigned int* Texture::GetImage() {
	return image;
}

int Texture::GetWidth() {
	return width;
}

int Texture::GetHeight() {
	return height;
}

GLuint Texture::Bind() {
	if (binded) return texture_id;

	glGenTextures(1, &texture_id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	return texture_id;
}