#pragma once

#include "PPC.h"
#include "Texture.h"

class ShadowMap {
public:
	PPC* ppc;
	Texture* depthBuffer;

public:
	ShadowMap(PPC* ppc);
};

