#pragma once

#include "TIFFImage.h"
#include "Texture.h"
#include <vector>
#include <Cg/cgGL.h>
#include <Cg/cg.h>

class CubeMap {
public:
	GLuint texture_id;
	std::vector<Texture*> textures;
	/*
	int width;
	int height;
	unsigned int* image;
	*/
	bool binded;

public:
	CubeMap(const char* filename);
	~CubeMap(void);

	GLuint Bind();
};

