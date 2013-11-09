#pragma once

#include "PPC.h"
#include "Texture.h"

class FrameBuffer;

class ShadowMap {
public:
	PPC* ppc;
	FrameBuffer* fb;
	Texture* depthBuffer;

public:
	ShadowMap(PPC* ppc, int w, int h);

	Texture* getTexture();
};

